/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 14时04分51秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	//非阻塞读终端
	char buf[20];
	//int fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	int fd = open("/dev/tty", O_RDONLY);
	if(fd < 0){
		perror("fd : ");
		exit(1);
	}

	printf("fd:%d\n", fd);
	printf("STDOUT_FILENO:%d\n", STDOUT_FILENO);
	printf("STDIN_FILENO:%d\n", STDIN_FILENO);
	printf("STDERR_FILENO:%d\n", STDERR_FILENO);
	
	//char strs[] = "hello unblock read"; 
	//write(fd, strs, strlen(strs));
	int count = read(fd, buf, 20);
	if(count > 0){
		write(STDOUT_FILENO, buf, 20);
	}
	close(fd);

	return 0;
}

