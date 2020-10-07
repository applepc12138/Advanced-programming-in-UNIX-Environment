/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月27日 星期四 11时44分15秒
 ************************************************************************/

#include <stdio.h>

//同一进程组的各进程接收来自同一终端的各种信号
//组长进程标识:其进程组ID==其进程ID
//pid_t getpgrp();等同于getpgid(0);
//pid_t getpgid(pid_t pid);
//setpgid()可以加入一个现有的进程组或创建一个新进程组
//如改变子进程为新的组，应在fork后，exec前使用
//非root进程只能改变自己或自己创建的子进程的进程组id
//int setpgid(pid_t pid, pid_t pgid);

int main(int argc, char *argv[])
{
	
	return 0;
}
