/*************************************************************************
	> File Name: processpool.cpp
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月31日 星期六 09时53分04秒
 ************************************************************************/
#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <strings.h>

static int sig_pipefd[2];

static int setnonblocking(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flag);
	return flag;
}

//向epoll注册事件
static void addfd(int epfd, int fd, int events)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = events;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

static void removefd(int epfd, int fd)
{
	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
}

static void sig_handler(int sig)
{
	//统一事件源，接收到信号并不做处理，而是向进程发送通知
	int save_errno = errno;
	send(sig_pipefd[1], (char *)&sig, 1, 0);
	errno = save_errno;	//????
}

static void add_sighandler(int sig, void (*handler)(int), bool restart = true)
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = handler;
	if(restart)
		act.sa_flags |= restart;
	//阻塞处理其他信号
	sigfillset(&act.sa_mask);
	sigaction(sig, &act, NULL);
}

class process{
	public:
		process() : pid(-1){}	
	private:
		pid_t pid;
		int m_pipefd[2];
};

//模板参数是处理逻辑任务的类
template<typename T>
class processpool{
	public:
		processpool(const processpool&) = delete;
		processpool(const processpool&&) = delete;
	private:
		processpool(int listenfd, int process_num = 8);
	public:
		//单例模式
		static processpool *create(int listenfd, int process_num = 8);
		//启动进程池
		void run();
		void setup_sig_pipe();
	private:
		void run_parent();
		void run_child();
		
	private:
		static constexpr int MAX_PROCESS_NUM = 16;
		static constexpr int  USER_PRE_PROCESS = 65535;
		static constexpr int MAX_EVENT_NUM = 1000;
		int process_num;
		//子进程序号，父进程index为-1
		int index;
		int epollfd;
		int listenfd;
		bool stop;
		//子进程描述信息 
		std::vector<process> sub_process;
};

template<typename T>
processpool<T>::processpool(int listenfd, int process_num)
	:listenfd(listenfd), process_num(process_num),index(-1), stop(false)
{
	assert(process_num > 0 && process_num <= MAX_PROCESS_NUM);

	sub_process.resize(process_num);
	
	//创建子进程
	for(int i = 0; i < process_num; ++i){
		//父进程与每个子进程都会由一个管道来保持联系
		socketpair(PF_UNIX, SOCK_STREAM, 0, sub_process[i].pipefd);
		sub_process[i].pid = fork();

		if(sub_process[i].pid > 0){//父进程中
			close(sub_process[i].pipefd[1]);
			continue;
		}
		else{//子进程中
			close(sub_process[i].pipefd[0]);
			index = i;
			return;
		}
	}
}

template<typename T>
processpool<T> *processpool<T>::create(int listenfd, int process_num)
{
		static processpool instance(listenfd, process_num);
		return instance;	
}

template<typename T>
void processpool<T>::setup_sig_pipe()
{
	epollfd = epoll_create(5);
	assert(epollfd != -1);

	socketpair(PF_UNIX, SOCK_STREAM, 0, sig_pipefd);
	
	setnonblocking(sig_pipefd[1]);
	addfd(epollfd, sig_pipefd[0], EPOLLIN | EPOLLET);
	add_sighandler(SIGCHLD, sig_handler);
	add_sighandler(SIGTERM, sig_handler);
	add_sighandler(SIGINT, sig_handler);
	add_sighandler(SIGPIPE, sig_handler);
}

template<typename T>
void processpool<T>::run()
{
	if(index < 0){//在父进程中
		run_parent();	
	}
	else{//在子进程中
		run_child();
	}
}

//每个子进程单独维护一个epollfd,需要在此epollfd上接受新连接并且处理每个连接请求
template<typename T>
void processpool<T>::run_child()
{
	setup_sig_pipe();//?????
	//父进程通过此fd与子进程通信
	int pipefd = sub_process[index].m_pipefd[1];
	//
	addfd(epollfd, pipefd, EPOLLET | EPOLLIN);

	epoll_event events[MAX_EVENT_NUM];
	
	T* users = new T[USER_PRE_PROCESS];

	while(!stop){
		//所有子进程都会阻塞在这里
		//当子进程被唤醒时(有文件描述符就绪时),子进程需要处理以下几种事件
		//
		int readnum = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);
		if((readnum < 0) && (errno != EINTR)){//(readnum < 0 && errno == EINTR)说明epoll_wait被信号中断
			printf("epoll failure\n");
			break;
		}
		
		for(int i = 0; i < readnum; ++i){
			int socketfd = events[i].data.fd;
			//父进程向子进程发送信息，通知子进程有新连接到达
			if(socketfd == pipefd && (events[i].events & EPOLLIN)){
				int client = 0;
				//接收父进程发送的一个字节数据，有什么用????
				int ret = recv(socketfd, (char *)&client, sizeof(client), 0);
				if((ret < 0 && errno != EINTR) || ret == 0)
					continue;
				else{//有新连接到达
					struct scokaddr_in clientaddr;
					socklen_t clientaddrlen;
					int confd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
					
					addfd(epollfd, confd, EPOLLET | EPOLLIN);
					//
					//users[confd].init(epollfd, confd, clientaddr);
				}
			}	
			else if(socketfd == sig_pipefd[0] && (events[i].events & EPOLLIN)){	//子进程接收到信号，信号处理函数通过管道通知子进程
				int sig;
				char signals[1024];
				int ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
				if(ret <= 0)
					continue;
				else{
					for(int i = 0; i < ret; ++i){
						switch(signals[i]){
							case SIGCHLD:
								pid_t pid;
								int stat;
								//what????
								while((pid = waitpid(-1, &stat, WNOHANG)) > 0)//等待回收子进程，若无立即返回
									continue;
								break;
							case SIGTERM:
							case SIGINT:
								stop = true;
								break;
							default:
								break;
						}
					}
				}
			}
			else if(events[i].events & EPOLLIN){//其他可读数据，客户端请求
				//users[socketfd].process();
			}
		}
	}	
	delete[] users;
	users = NULL;
	close(pipefd);
	close(epollfd);
}

template<typename T>
void processpool<T>::run_parent()
{
	setup_sig_pipe();//????
	addfd(epollfd, listenfd, EPOLLIN | EPOLLET);//注意！！！！子进程中没有这一行

	epoll_event events[MAX_EVENT_NUM];
	int sub_process_counter = 9;
	int new_conn = 1;
	int ret = -1;

	while(!stop){
		int readnum = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);
		if(readnum < 0 && errno != EINTR){
			printf("epoll fail\n");
			break;
		}

		for(int i = 0; i < readnum; ++i){
			int socketfd  = events[i].data.fd;
			if(socket == listenfd){//有新连接到达, Round Robin方式分配个一个子进程处理

			}
			else if(socketfd == sig_pipefd[0] && (events[i].events & EPOLLIN)){//父进程处理接收到的信号
				int sig;
				char signals[1024];
				ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
				if(ret <= 0)
					continue;
				else{
					for(int i= 0; i < ret; ++i){
						switch (signals[i])
						{
							case SIGCHLD:
								pid_t pid;
								int stat;
								while((pid = waitpid(-1, &stat, WNOHANG)) > 0){
									for(int i = 0; i < this->process_num; ++i){

									}
								}
								break;
							case SIGTERM:
							case SIGINT:
								//父进程收到终止信号，则杀死所有子进程，并等待他们结束
								printf("kill all the child now\n");
								for(int i= 0; i < this->process_num; ++i){
									int pid = this->sub_process[i].pid;
									if(pid != -1)
										kill(pid, SIGTERM);
								}
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
}

#endif
