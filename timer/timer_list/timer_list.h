/*************************************************************************
	> File Name: timer_list.h
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月16日 星期五 18时09分22秒
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctime>

#define BUF_SIZE 64

class MyTimer;

//当服务器接受新连接后保存的客户端信息
struct client_data{
	struct sockaddr_in addr;
	int confd;
	char buf[BUF_SIZE];
	MyTimer *timer;//当客户端有数据到达时需要更新其定时任务
};


//定时任务类
class MyTimer{
	friend class TimerList;//非某个模板类的特定实例,无需前置声明
	public:
		using cbptr = void (*)(client_data*);
	public:
		MyTimer(){}
		MyTimer(MyTimer *pre, MyTimer *next):
			_pre(pre), _next(next){}
		MyTimer(time_t et, cbptr cb):
			expireTime(et), call_back{cb}{}
		~MyTimer(){}

	private:
		time_t expireTime;//超时时间(绝对时间)
		cbptr call_back;
		client_data *user_data;
		MyTimer *_pre = nullptr;
		MyTimer *_next = nullptr;
};

//定时任务链表
class TimerList{
	public:
		TimerList(){}
		~TimerList();

	public:
		void addTimer(MyTimer *timer);
		void deleteTimer(MyTimer *timer);
		void adjust(MyTimer *timer);
		void tick();

	private:
		//辅助函数,将timer插入到以head为头结点的链表的合适位置
		void addTimer(MyTimer *head, MyTimer *timer);
	
	private:
		MyTimer *head = nullptr;
		MyTimer *tail = nullptr;
};
