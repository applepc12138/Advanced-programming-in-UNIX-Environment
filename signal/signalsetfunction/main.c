#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

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

