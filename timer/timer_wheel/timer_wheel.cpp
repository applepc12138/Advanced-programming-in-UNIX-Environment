/*************************************************************************
	> File Name: timer_wheel.cpp
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月19日 星期一 20时43分33秒
 ************************************************************************/

#include "timer_wheel.h"

void TimerWheel::addTimer(int timeout)
{
	if(timeout < 0)
		return;
	int ticks = 0;
	if(timeout < SI)
		ticks = 0;
	else 
		ticks = timeout / SI;
	int round = ticks / _n;//多少圈后被触发
	//计算定时器落在的槽位置	
	int slot;
	if(round < 1){
		slot = _cur + ((_cur + ticks) % _n);
	}
	else {
		slot = _cur + ticks % _n;		
	}
	Timer *timer = new Timer(round, slot);
	if(wheel[slot] == nullptr){
		wheel[slot] = timer;
	}
	else {
		wheel[slot]->_pre = timer;
		timer->_next = wheel[slot];
		wheel[slot] = timer;
	}
}

void TimerWheel::deleteTimer(Timer *timer)
{
	if(timer == nullptr)
		return;
	if(timer->_pre == nullptr){
		int slot = timer->_slot;
		wheel[slot] = timer->_next;
		wheel[slot]->_pre = nullptr;
	}
	else{
		timer->_pre->_next = timer->_next;
		if(timer->_next)
			timer->_next->_pre = timer->_pre;
	}
	delete timer;
}

void TimerWheel::adjust(Timer *timer, int timeout)
{

}

void TimerWheel::tick()
{
	Timer *next = wheel[_cur];
	while(next){
		if(next->_round > 0){
			--(next->_round);
			next = next->_next;
		}
		else{
			next->call_back(next->clientData);
			Timer *tmp = next;
			next = next->_next;
			deleteTimer(tmp);
		}
	}
}
