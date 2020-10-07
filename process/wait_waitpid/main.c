/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月26日 星期三 11时24分21秒
 ************************************************************************/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//若其所有子进程正在运行,则阻塞
//若有一个子进程已终止,父进程获取其状态并返回
//若无任何子进程,则出错返回错返回
//pid_t wait(int *wstatus);
//pid_t waitpid(pid_t pid, int *wstatus,int options);
//返回值:成功则返回回收子进程id,否则返回0或-1

int main(int argc, char *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
		perror("fork fail");
	else if(pid == 0){//子进程
		if((pid = fork()) < 0)
			perror("fork fail");
		else if(pid > 0)
			exit(1);
		sleep(2);
		printf("second child, parent pid = %ld\n", (long)getppid());
		exit(1);
	}
	if(waitpid(pid, NULL, 0) != pid)//回收第一个子进程
		perror("waitpid error");
	exit(0);

	return 0;
}

