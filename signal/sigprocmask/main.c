/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月14日 星期三 18时30分33秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

// 调用函数sigprocmask可以读取或更改进程的信号屏蔽字
// int sigprocmask(int how, const sigset_t *set, sigset_t *oset);
// 返回值：若成功则为0，若出错则为-1
// how参数:
// SIG_BLOCK	set包含了我们希望添加到当前信号屏蔽字的信号，相当于mask=mask|set
// SIG_UNBLOCK	set包含了我们希望从当前信号屏蔽字中解除阻塞的信号，相当于mask=mask&~set
// SIG_SETMASK	设置当前信号屏蔽字为set所指向的值，相当于mask=set
// 若调用sigprocmask解除了对当前若干个未决(挂起)信号的阻塞，则在sigprocmask返回前，至少将其中一个信号递达


sem_t sem;

void signal_handler(int sig);

int main(int argc, char *argv[])
{
	sem_init(&sem, 0, 0);

	struct sigaction act;
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, NULL);

	alarm(5);
	
	sem_wait(&sem);
	return 0;
}

//信号处理函数
void signal_handler(int sig)
{
	sigset_t set, oldset;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	sigprocmask(SIG_BLOCK, &set, &oldset);
	
	//读取当前进程的信号屏蔽字
	printf("oldsets:\n");
	for(int i = 1; i < NSIG; ++i){
		if(sigismember(&oldset, i))
			putchar('1');
		else 
			putchar('0');
	}
	printf("\nover\n");
	sem_post(&sem);

}

