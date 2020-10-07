/*************************************************************************
	> File Name: signal.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年07月07日 星期二 20时16分03秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<signal.h>
#include<unistd.h>

void int_handler(int signum)
{
	printf("Got a int signal %d\n", signum);
}


int main(int argc, char *argv[])
{
	signal(SIGINT, int_handler);
	printf("enter to the sleep.\n");
	sleep(20);
	printf("sleep is over, main over.\n");
	return 0;
}

