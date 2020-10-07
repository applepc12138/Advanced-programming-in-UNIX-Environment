/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月27日 星期四 20时47分28秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

//fifo可以在两个不相关的进程间传输数据,多个写进程向一个fifo写数据是常见的
//FIFO可以一个读端，多个写端；也可以一个写端，多个读端。
//类似于creat(const char *path, mode_t mode)创建一个文件
//参数mode指定权限位
//int mkfifo(const char *path, mode_t mode);
//int mkfifoat(int fd, const char *path, mode_t mode);


int main()
{
	char buf[64];
	if(mkfifo("FIFO", S_IRWXU) == -1){
		perror("create fifo fail");
		return 0;
	}
	int fd = open("FIFO", O_RDWR);
	if(fd == -1){
		perror("opeo FIFO fail");
		return 0;
	}
	pid_t pid;
	if((pid = fork()) < 0){
		perror("fork fail");
		return 0;
	}
	else if(pid == 0){
		int count = read(fd, buf, 64);
		write(STDOUT_FILENO, buf, count);
	}
	else if(pid > 0){
		write(fd, "write some message", 18);
		wait(NULL);
	}

	return 0;
}

