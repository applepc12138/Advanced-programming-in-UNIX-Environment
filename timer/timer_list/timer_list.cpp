/*************************************************************************
	> File Name: timer_list.cpp
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月16日 星期五 19时17分35秒
 ************************************************************************/

#include "timer_list.h"

void TimerList::addTimer(MyTimer *timer){
	if(timer == nullptr)
		return;
	if(head == nullptr){
		head = tail = timer;
		return;
	}
	if(timer->expireTime < head->expireTime){
		timer->_next = head;
		head = timer;
	}
	else 
		addTimer(head, timer);
	
}

void TimerList::addTimer(MyTimer *head, MyTimer *timer){
	auto tmp = head;
	while(tmp){
		if(tmp->expireTime >= timer->expireTime)
			break;
		tmp = tmp->_next;
	}
	if(tmp == nullptr){//timer.expireTime大于链表上所有节点
		tail->_next = timer;
		timer->_pre = tail;
		tail = timer;
		return;
	}
	timer->_next = tmp;
	tmp->_pre = timer;
	tmp = timer->_next;
	timer->_pre = tmp;
	tmp->_next = timer;
}

void TimerList::deleteTimer(MyTimer *timer){
	if(timer == nullptr)
		return;
	if(head  == timer && timer == tail){//删除链表中仅有的一个节点
		delete timer;
		head = tail = nullptr;
	}
	else if(head == timer){//要删除的是头结点
		head = timer->_next;
		head->_pre = nullptr;
		delete timer;
	}
	else if(tail == timer){//要删除的是尾节点
		tail = tail->_pre;
		tail->_next = nullptr;
		delete timer;
	}
	else {
		timer->_next->_pre = timer->_pre;
		timer->_pre->_next = timer->_next;
		delete timer;
	}
}

void TimerList::adjust(MyTimer *timer){
	if(timer == nullptr)
		return;
	//定时任务在链表尾或其超时时间仍小于后一个定时任务
	auto next = timer->_next;
	if(next == nullptr || 
			timer->expireTime < next->expireTime)
		return;

	if(head == timer){
		head = head->_next;
		head->_pre = nullptr;
		addTimer(timer);
	}
	else{
		//将timer取出再重新插入
		timer->_next->_pre = timer->_pre;
		timer->_pre->_next = timer->_next;
		addTimer(timer->_next, timer);
	}
	
}

void TimerList::tick(){
	//定时器每隔固定时间发出信号,信号处理函数接收信号后利用
	//管道通知主程序，主程序最终调用此函数
	if(head == nullptr)
		return;
	decltype(head) tmp;
	auto cur = head;
	time_t curTime = time(NULL);
	while(cur){
		if(cur->expireTime > curTime)
			break;
		cur->call_back(cur->user_data);
		tmp = cur;		
		cur = cur->_next;
		deleteTimer(tmp);
	}
}

TimerList::~TimerList(){
	auto tmp = head;
	while(head){
		head = head->_next;
		delete tmp;
		tmp = head;
	}
}


