/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月14日 星期三 18时34分40秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>

// int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
// struct sigaction {
//     void   (*sa_handler)(int);
//     void   (*sa_sigaction)(int, siginfo_t *, void *);
//     sigset_t  sa_mask;
//     int sa_flags;
//     void   (*sa_restorer)(void);
// };
// sa_handler : 早期的捕捉函数
// sa_sigaction : 新添加的捕捉函数，可以传参 , 和sa_handler互斥，两者通过sa_flags选择采用哪种捕捉函数
// sa_mask : 在执行捕捉函数时，设置阻塞其它信号，sa_mask | 进程阻塞信号集，退出捕捉函数后，还原回原有的阻塞信号集
// sa_flags : SA_SIGINFO 或者 0
// sa_restorer : 保留，已过时

int main(int argc, char *argv[])
{

	return 0;
}
