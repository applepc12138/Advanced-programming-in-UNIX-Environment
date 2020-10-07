/*************************************************************************
	> File Name: fork.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年07月08日 星期三 22时34分54秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int main(int argc, char *argvp[])
{
	int n = 10;
	pid_t pid = fork();
	
	if(pid > 0){//父进程
		printf("I am parent\n");
		printf("the parent var n address is %p\n", &n);
		printf("the parent var n value is %d\n", n);
		printf("parent process id is %d\n", getpid());
		printf("parent's father process id is %d\n", getppid());
		printf("parent process usr id is %d\n", getuid());
		sleep(1);

	}
	else if(pid == 0){//子进程
		printf("I am child\n");
		printf("the child var n address is %p\n", &n);
		++n;
		printf("the child var n value is %d\n", n);
		printf("child process id is %d\n", getpid());
		//子进程返回getppid()可能与父进程id不同(当子进程变成孤儿进程)
		printf("child's parent process id is %d\n", getppid());
		printf("child process usr id is %d\n", getuid());
	}
	else{
		printf("error\n");
	}

	return 0;
}
