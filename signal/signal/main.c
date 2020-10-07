/*************************************************************************
	> File Name: sendsignal.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年07月08日 星期三 13时42分51秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

//如果进程在执行一个低速系统调用而阻塞期间捕获到一个信号，则该系统调用就被中断不再执行，该系统调用返回出错，其errno设置为EINTR
//
//void (*signal)(int signo, void(*func)(int))(int);
//返回值：成功返回之前的信号配置函数，否则返回SIG_ERR

void sig_handler(int signum)
{
	printf("got a signal\t %d\n", signum);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_handler);
	while(1)
		sleep(50);
	return 0;
}
