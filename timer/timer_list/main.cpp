/*************************************************************************
	> File Name: main.cpp
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月17日 星期六 12时58分41秒
 ************************************************************************/

#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <signal.h>
#include <cassert>
#include "timer_list.h"

constexpr int FD_LIMIT = 65535;
constexpr int MAX_EVENT_NUM = 1024;//两者大小区别???
constexpr int TIMESLOT = 5;

static int pipefd[2];
static int epollfd;
static TimerList timerList;

void setnonblock(int confd);
void addfd(int ep, int confd, int events);
void signal_handler(int sig);
void addsigToHandle(int sig, void (*sighandler)(int));

//当超时链表中的定时任务到期时，调用此函数需要相关联的客户端信息
void cb_func(client_data *arg)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, arg->confd, NULL);
	close(arg->confd);
	printf("client sockfd: %d close\n", arg->confd);
}

int main(int argc, char *argv[])
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	int stop_server = 0;
	int timeout = 0;
	
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	serveraddr.sin_port = htons(65535);
	bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	setnonblock(listenfd);
	listen(listenfd, 128);

	epollfd = epoll_create(5);
	addfd(epollfd, listenfd, EPOLLIN);
	
	//管道
	int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
	assert(ret != -1);
	//setnonblock(pipefd[0]);
	setnonblock(pipefd[1]);
	addfd(epollfd, pipefd[0], EPOLLIN);
	
	//用于保存epoll_wait返回的就绪事件
	struct epoll_event events[MAX_EVENT_NUM];

	//添加需要处理的信号
	addsigToHandle(SIGALRM, signal_handler);
	addsigToHandle(SIGTERM, signal_handler);
	alarm(TIMESLOT);

	//用于保存客户端信息
	client_data *users = new client_data[FD_LIMIT];

	while(!stop_server){
		int readynum = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);//被信号终端此系统调用并不会自动重启
		printf("epoll_wait over readynum :%d\n", readynum);
	   /*  if(errno == EINTR) */
			/* printf("errno : EINTR\n"); */
		if((readynum < 0) && (errno != EINTR)){
			printf("epoll fail\n");
			break;
		}

		for(int i = 0; i < readynum; ++i){
			int sockfd = events[i].data.fd;
			if(sockfd == listenfd){//处理新到客户端连接
				struct sockaddr_in clientaddr;
				socklen_t clientaddrlen = sizeof(clientaddr);
				int confd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientaddrlen);

				setnonblock(confd);
				addfd(epollfd, confd, EPOLLIN);

				users[confd].addr = clientaddr;
				users[confd].confd = sockfd;
				users[confd].timer = new MyTimer(time(NULL) + 3 * TIMESLOT, cb_func, &users[confd]);
				//将此sockfd的定时任务添加到TimerList中
				timerList.addTimer(users[confd].timer);
			}
			else if((sockfd == pipefd[0]) && events[i].events  & EPOLLIN){//处理信号事件
				char signals[1024];//????:解释:进程陷入内核态后向用户态返回时，内核检查收到的信号可能不止一个
				int ret = recv(sockfd, signals, sizeof(signals), 0);
				if(ret == -1){
					//handle the error 
					continue;
				}
				else if(ret == 0)
					continue;
				else{
					for(int i = 0; i < ret; ++i){
						switch(signals[i]){
							case SIGTERM:
								stop_server = 1;
								break;
							case SIGALRM:
								timeout = 1;
								break;
						}
					}	
				}
			}
			else if(events[i].events & EPOLLIN){//接收客户端发送数据
				//清除上次接收数据影响
				bzero(users[sockfd].buf, BUF_SIZE/*sizeof(users[sockfd].buf)*/);
				int ret = recv(sockfd, users[sockfd].buf, BUF_SIZE, 0);

				printf("get %d bytes of client data : %s \t from sockfd:%d\n",
						ret, users[sockfd].buf, sockfd);
				MyTimer *timer = users[sockfd].timer;
				if(ret < 0){
					if(errno != EAGAIN){//发生读错误,则关闭连接，并移除相关的定时任务
						cb_func(&users[sockfd]);//timer->call_back(&users[sockfd]);
						timerList.deleteTimer(timer);
					}
				}
				else if(ret == 0){//客户端已关闭连接
					cb_func(&users[sockfd]);//timer->call_back(&users[sockfd]);
					timerList.deleteTimer(timer);
				}
				else{//业务处理
					
					//更新此sockfd的定时任务
					timer->setExpireTime(time(NULL) + 3 * TIMESLOT);					
					timerList.adjust(timer);
					printf("adjust timer success\n");
				}
			}
		}
		if(timeout == 1){//处理超时连接
			timerList.tick();
			alarm(TIMESLOT);
			timeout = 0;
		}
	}

	close(listenfd);
	close(pipefd[0]);
	close(pipefd[1]);
	delete[] users;
	
	return 0;
}

void setnonblock(int confd)
{
	int flag = fcntl(confd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(confd, F_SETFL, flag);
}

void addfd(int ep, int confd, int events)
{
	struct epoll_event epevent;
	epevent.events = events;
	epevent.data.fd = confd;
	epoll_ctl(ep, EPOLL_CTL_ADD, confd, &epevent);	
}

void signal_handler(int sig)
{
	int msg = sig;//???
	send(pipefd[1], (char *)&msg, 1, 0);//????
	printf("signal_handler over\n");
}

void addsigToHandle(int sig, void (*sighandler)(int))
{
	struct sigaction sigact;
	//此标志对epoll_wait poll select accept recv recvfrom send sendto sendmsg等系统调用无效
	sigact.sa_flags |= SA_RESTART;//???:解释:当进程处于阻塞状态的系统调用时到接收信号，默认情况系统调用被中断，设置此标志自动重启被信号中断的系统调用
	sigfillset(&sigact.sa_mask);//???
	sigact.sa_handler = sighandler;
	sigaction(sig, &sigact, NULL);
}
