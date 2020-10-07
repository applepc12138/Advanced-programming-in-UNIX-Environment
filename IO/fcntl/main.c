/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 14时56分08秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//fcntl()可以改变已经打开文件的属性
//int fcntl(int fd, int cmd);
//int fcntl(int fd, int cmd, long arg);
//int fcntl(int fd, int cmd, struct flock *lock)
//1.复制已有的文件描述符 cmd=F_DUPFD
//2.获取/设置文件描述符标志 cmd=F_GETFD F_SETFD
//3.获取/设置文件状态标志 cmd = F_GETFL F_SETFL
int main()
{
	char buf[10];
	int flag = fcntl(STDIN_FILENO, F_GETFL);
	flag |= O_NONBLOCK;
	if(fcntl(STDIN_FILENO, F_SETFL, flag) == -1){
		perror("fcntl set fail");
		exit(1);
	}
	int count = 0;
	if((count = read(STDIN_FILENO, buf, 10)) < 0){
		perror("read fail");
		exit(1);
	}
	write(STDOUT_FILENO, buf, count);
	return 0;
}
