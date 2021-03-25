/*************************************************************************
	> File Name: timer_wheel.h
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月19日 星期一 16时17分59秒
 ************************************************************************/

#ifndef TIMIER_WHEEL_H
#define TIMIER_WHEEL_H

#include <time.h>
#include <netinet/in.h>
#include <vector>

constexpr int BUF_SZIE = 128;

class Timer;

struct ClientData{
	sockaddr_in clientaddr;
	int sockfd;
	char buf[BUF_SZIE];
	Timer *timer;
};

class Timer{
	friend class TimerWheel;
public:
	using cbptr = void (*)(ClientData *);
public:
	Timer(){}
	Timer(int round, int slot, cbptr cb = nullptr)
		:_round(round), _slot(slot), call_back(cb){}
	Timer(Timer *pre, Timer *next, cbptr cb = nullptr)
		:_next(next), _pre(pre), call_back(cb){}
	~Timer(){}
private:
	int _round = 0;
	int _slot = 0;
	Timer *_next = nullptr;
	Timer *_pre = nullptr;
	ClientData *clientData = nullptr;
	cbptr call_back = nullptr;
};

class TimerWheel{
public:
	TimerWheel()
	{
		for(int i = 0; i < _n; ++i)
			wheel[i] = nullptr;
	}
	TimerWheel(int n, int si)
		:_n(n), SI(si)
	{
		for(int i = 0; i < _n; ++i)
			wheel[i] = nullptr;
	}
	~TimerWheel(){}

public:
	void addTimer(int timeout);
	void deleteTimer(Timer *timer);
	void adjust(Timer *timer, int timeout);
	void tick();
		
private:
	int _n = 30;//时间轮上槽的个数
	int _cur = 0;//当前指向槽的位置
	std::vector<Timer *> wheel;
	int SI = 1;//每一秒时间轮转动一格
};

#endif
