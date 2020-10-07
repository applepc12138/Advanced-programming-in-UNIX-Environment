/*************************************************************************
	> File Name: server.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月01日 星期四 19时35分35秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include  <sys/time.h>
#include <strings.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/epoll.h>

// int epoll_create(int size)
//     size：告诉内核监听的数目

// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
//     epfd：为epoll_creat的句柄
//     op：表示动作，用3个宏来表示:
//         EPOLL_CTL_ADD(注册新的fd到epfd)
//         EPOLL_CTL_MOD(修改已经注册的fd的监听事件)
//         EPOLL_CTL_DEL(从epfd删除一个fd)
//     event：告诉内核需要监听的事件
//     struct epoll_event {
//         __uint32_t events; /* Epoll events */
//         epoll_data_t data; /* User data variable */
//     };
//     EPOLLIN ：表示对应的文件描述符可以读(包括对端SOCKET正常关闭)
//     EPOLLOUT：表示对应的文件描述符可以写
//     EPOLLPRI：表示对应的文件描述符有紧急的数据可读(这里应该表示有带外数据到来)
//     EPOLLERR：表示对应的文件描述符发生错误
//     EPOLLHUP：表示对应的文件描述符被挂断
//     EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的	 
//     EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里

// int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
//	  events：用来从内核得到事件的集合
//    maxevents：告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size
//    timeout：是超时时间
//        -1：阻塞
//        0：立即返回，非阻塞
//        >0：指定微秒返回值：成功返回有多少文件描述符就绪，时间到时返回0，出错返回-1

int main(int argc, char *argv[])
{
	//初始化socket
	int serverfd;
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("create socket fail\n");
		exit(1);
	}
	//绑定ip端口
	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	serversock.sin_addr.s_addr = htonl(INADDR_ANY);
	serversock.sin_port = htons(8888);
	if(bind(serverfd, (struct sockaddr *)&serversock,
				sizeof(serversock)) < 0){
		perror("server bind fail\n");
		exit(1);
	}
	
	listen(serverfd ,18);

	int epollfd;
	if((epollfd = epoll_create(6)) < 0){
		perror("create epoll fail\n");
		exit(1);
	}

	//添加需要监听的socket到epollfd中
	struct epoll_event temp;
	//ET边沿触发方式
	temp.events = EPOLLIN | EPOLLET;
	temp.data.fd = serverfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &temp);

	struct epoll_event ep[256];
	while(1){
		
		int readynum = epoll_wait(epollfd, ep, 256, -1);
		if(readynum < 0){
			perror("wait\n");
			exit(1);
		}
		for(int i = 0; i < readynum; ++i){
			if(ep[i].data.fd == serverfd){
				struct sockaddr_in clientsock;
				socklen_t clientsocklen = sizeof(clientsock);
				bzero(&clientsock, sizeof(clientsock));
				int confd = accept(serverfd, 
						(struct sockaddr *)&clientsock,
						&clientsocklen);
				//打印客户端ip端口号
				char clientaddr[32];
				printf("client ip:%s\tport:%d\n", 
						inet_ntop(AF_INET, 
							&clientsock.sin_addr.s_addr, 
							clientaddr, sizeof(clientaddr)),
						ntohs(clientsock.sin_port));
				
				//添加对客户端的监听
				//边沿触发方式
				//temp.events = EPOLLIN | EPOLLET;
				//水平触发方式
				temp.events = EPOLLIN;
				temp.data.fd = confd;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, 
							confd, &temp) < 0){
					perror("epoll add fail\n");
					exit(1);
				}
				//
			}
			else if(ep[i].events & EPOLLIN){//处理read事件
				char buf[512];
				//读数据，从confd中读一半数据
				int len = read(ep[i].data.fd, buf,
						5);
				if(len < 0)
					perror("read fail\n");
				else if(len == 0){//客户端断开连接
					epoll_ctl(epollfd, EPOLL_CTL_DEL, 
							ep[i].data.fd, NULL);
					close(ep[i].data.fd);
					ep[i].data.fd = -1;
				}
				else{
					for(int j = 0; j < len; ++j){
						buf[j] = toupper(buf[j]);
					}
					write(ep[i].data.fd, buf, len);
				}
			}
		}
		
	}

	return 0;
}
