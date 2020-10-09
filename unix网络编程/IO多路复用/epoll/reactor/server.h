/*************************************************************************
	> File Name: server.h
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月09日 星期五 14时27分23秒
 ************************************************************************/
#define BUFSIZE 4096
#define SERVER_PORT 8888

struct my_event{
	int fd;
	int event;
	void *arg;	//?
	void (*call_back)(int epfd, void *arg);
	int status;	//指示是否被注册到epoll红黑树中
	char buf[BUFSIZE];
	int buflen;
	long last_active;	//?

};

void event_set(struct my_event *myevent, int confd,
		int event,
		void (*cb)(int confd, void *arg),
		void *arg);

void event_add(int epfd, struct my_event *ev);

void event_del(int epfd, struct my_event *ev);

 void senddata(int epfd, void *arg);

 void recvdata(int epfd, void *arg);

 void acceptconnect(int epfd, void *arg);

 void initserversock(int epfd, short port);


