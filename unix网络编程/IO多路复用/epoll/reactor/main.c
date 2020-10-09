/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月09日 星期五 15时17分38秒
 ************************************************************************/

#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include "server.h"

#define MAX_EVENTS 1024

int main(int argc, char *argv[])
{
	int epfd = epoll_create(1024);
	initserversock(epfd, SERVER_PORT);
	struct epoll_event ep_event[MAX_EVENTS+1];
	while(1){
		int readynum = epoll_wait(epfd, ep_event, MAX_EVENTS + 1, 0);
		if(readynum < 0){
			perror("epoll_wait fail\n");
			exit(1);
		}
		for(int i = 0; i < readynum; ++i){
			struct my_event *ev = 
				(struct my_event *)ep_event[i].data.ptr;
			if(ep_event[i].events & EPOLLIN){
				ev->call_back(epfd, ev);
			}
			if(ep_event[i].events & EPOLLOUT){
				ev->call_back(epfd, ev);
			}
		}
	}

	return 0;
}

