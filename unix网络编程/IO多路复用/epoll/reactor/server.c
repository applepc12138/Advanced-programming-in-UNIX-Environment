/*************************************************************************
	> File Name: server.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com > Created Time: 2020年10月08日 星期四 21时14分59秒
 ************************************************************************/
#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <strings.h>
#include <ctype.h>
#include "server.h"

#define MAX_EVENTS 1024
#define BUFSIZE 4096

struct my_event events[MAX_EVENTS+1];

//构造一个my_event
void event_set(struct my_event *myevent, int confd,
		int event, 
		void (*cb)(int epfd, void *arg),
		void *arg)
{
	myevent->fd = confd;
	myevent->event = event;
	myevent->call_back = cb;
	myevent->status = 0;
	bzero(myevent->buf, sizeof(myevent->buf));
	myevent->buflen = 0;
	myevent->last_active = time(NULL);//调用此函数的时间
}

//注册event到红黑树中
void event_add(int epfd, struct my_event *ev)
{
	struct epoll_event epv = {0, {0}};
	epv.data.ptr = ev;
	epv.events = ev->event;
	int op = 0;
	if(ev->status == 0)//未在红黑树中，则注册
		op = EPOLL_CTL_ADD;
	else 
		op = EPOLL_CTL_MOD;
	
	if(epoll_ctl(epfd, op, ev->fd, &epv) < 0){
		perror("epoll_ctl fail\n");
		exit(1);
	}
	ev->status = 1;
}

//将事件从红黑树中删除
void event_del(int epfd, struct my_event *ev)
{
	if(ev->status == 1)
		epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, NULL);
}

//回调函数，由工作线程调用
void senddata(int epfd, void *arg)
{
	struct my_event *ev = (struct my_event *)arg;
	int len = send(ev->fd, ev->buf, ev->buflen, 0);
	if(len < 0){
		perror("send data fail");
		exit(1);
	}
	event_del(epfd, ev);
	//注册读事件到红黑树
	event_set(ev, ev->fd, EPOLLIN, 
			recvdata, ev->arg);
	event_add(epfd, ev);
}

//回调函数，由工作线程调用
void recvdata(int epfd, void *arg)
{
	struct my_event *ev = (struct my_event *)arg;
	int len = recv(epfd, ev->buf, ev->buflen, 0);
	if(len < 0){
		perror("recv fail\n");
		exit(1);
	}
	else if(len == 0){
		close(ev->fd);//?
	}
	//处理数据
	for(int i = 0; i < len; ++i)
		ev->buf[i] = toupper(ev->buf[i]);
	ev->buf[len] = '\0';
	ev->buflen = len;

	event_del(epfd, ev);
	//注册写事件到红黑树
	event_set(ev, ev->fd, EPOLLOUT, 
			senddata, ev->arg);
	event_add(epfd, ev);
}

//回调函数，工作线程调用
void acceptconnect(int epfd, void *arg)
{
	struct my_event *ev = (struct my_event *)arg;
	struct sockaddr_in clientsock;
	socklen_t clientsocklen = sizeof(clientsock);
	int confd = accept(ev->fd, (struct sockaddr *)&clientsock,
			&clientsocklen);
	char clientaddr[32];
	printf("client ip : %s\tport : %d\n",
			inet_ntop(AF_INET, &clientsock.sin_addr,
				clientaddr, sizeof(clientaddr)),
			ntohs(clientsock.sin_port));

	//设置confd为非阻塞
	int flag = fcntl(confd, F_GETFL);
	flag |= O_NONBLOCK;
	if(fcntl(confd, F_SETFL, flag) < 0){
		perror("fcntl set nonblock fail\n");
		exit(1);
	}
	int i = 0;
	for(; i < MAX_EVENTS; ++i){
		if(events[i].status == 1)
			break;
	}
	if(i == MAX_EVENTS){
		printf("max connect limit\n");
		exit(1);
	}
	//将confd添加到红黑树中
	event_set(&events[i], confd, EPOLLIN, recvdata, 
			&events[i]);
	event_add(epfd, &events[i]);
}

void initserversock(int epfd, short port)
{
	int listenfd;
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("create socket fail\n");
		exit(1);
	}

	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1",
			&serversock.sin_addr.s_addr);
	serversock.sin_port = htons(SERVER_PORT);	

	if(bind(listenfd, (struct sockaddr *)&serversock, 
				sizeof(serversock)) < 0){
		perror("bind listenfd fail\n");
		exit(1);
	}

	listen(listenfd, 128);

	//将listenfd注册到红黑树中
	event_set(&events[MAX_EVENTS], listenfd, EPOLLIN, 
			acceptconnect, &events[MAX_EVENTS]);
	
	event_add(epfd, &events[MAX_EVENTS]);
}
