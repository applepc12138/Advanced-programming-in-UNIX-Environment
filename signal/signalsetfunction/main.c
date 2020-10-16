#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// sigset_t为信号集,可sizeof(sigset_t)察看
// int sigemptyset(sigset_t *set);
// int sigfillset(sigset_t *set);
// int sigaddset(sigset_t *set, int signo);
// int sigdelset(sigset_t *set, int signo);
// int sigismember(const sigset_t *set, int signo);

void print_sigset(sigset_t *set)
{
	int i;
	for(i = 1; i < NSIG; ++i){
		if(sigismember(set, i))
			putchar('1');
		else
			putchar('0');
	}
	putchar('\n');
}

int main(void)
{
	sigset_t foobar;

	sigemptyset(&foobar);
	sigaddset(&foobar, SIGINT);
	sigaddset(&foobar, SIGQUIT);
	sigaddset(&foobar, SIGUSR1);
	sigaddset(&foobar, SIGRTMIN);

	print_sigset(&foobar);

	return 0;
}

