/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月09日 星期五 15时17分38秒
 ************************************************************************/

#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"

#define MAX_EVENTS 1024

struct Arg{
	int epfd;
	struct my_event *ev; 
};

void  *thread_hander(void *arg)
{
	struct Arg *tmp = (struct Arg *)arg;
	tmp->ev->call_back(tmp->epfd, tmp->ev);
	return NULL;
}

int main(int argc, char *argv[])
{
	int epfd = epoll_create(1024);
	initserversock(epfd, SERVER_PORT);
	struct epoll_event ep_event[MAX_EVENTS+1];
	while(1){
		int readynum = epoll_wait(epfd, ep_event, MAX_EVENTS + 1, -1);
		printf("readynum = %d\n", readynum);
		if(readynum < 0){
			perror("epoll_wait fail\n");
			exit(1);
		}
		struct Arg arg;
		for(int i = 0; i < readynum; ++i){
			arg.epfd =  epfd;
			arg.ev = (struct my_event *)ep_event[i].data.ptr;
			
			if(ep_event[i].events & EPOLLIN){
				// pthread_t tid;
				// pthread_create(&tid, NULL, thread_hander, 
				//         (void *)(&arg));
				// pthread_detach(tid);
				arg.ev->call_back(epfd, arg.ev);
			}

			if(ep_event[i].events & EPOLLOUT){
				// pthread_t tid;
				// pthread_create(&tid, NULL, thread_hander, 
				//         (void *)(&arg));
				//      pthread_detach(tid);
				arg.ev->call_back(epfd, arg.ev);
			}
		}
	}

	return 0;
}

