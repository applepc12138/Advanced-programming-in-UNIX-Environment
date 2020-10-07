/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月27日 星期四 10时32分50秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//int execl(const char *path, const char *arg, ...);
//int execlp(const char *file, const char *arg, ...);
//int execle(const char *path, const char *arg, ..., char *const envp[]);
//int execv(const char *path, char *const argv[]);
//int execvp(const char *file, char *const argv[]);
//int execve(const char *path, char *const argv[], char *const envp[]);

int main(int argc, char *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0){
		perror("fork fail");
		return 0;
	}
	if(pid > 0){
		wait(NULL);
		printf("recycle over\n");
	}
	else{
		execl("~/c_project/unix环境高级编程/IO/ioctl/ioctl", "ioctl", NULL);
		perror("exec fail");
	}
	return 0;
}

