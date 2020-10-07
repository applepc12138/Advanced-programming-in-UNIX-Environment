/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月26日 星期三 18时41分12秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

static void help(char *str)
{
	int c;
	setbuf(stdout, NULL);
	for(char *ptr = str; (c = *ptr++) != '\0'; )
		putc(c, stdout);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
		perror("fork fail");
	if(pid == 0)
		help("output from child\n");
	else if(pid > 0)
		help("output from parent\n");
	return 0;
}

