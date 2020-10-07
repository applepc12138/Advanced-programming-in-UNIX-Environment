/*************************************************************************
	> File Name: read.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月22日 星期六 22时12分27秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// ssize_t read(int fd, void *buf, size_t count);
// read()可以读文件、终端、网络、管道 
// 返回值：成功返回读取的字节数，出错返回-1并设置errno，如果在调read之前已到达文件末尾，则这次read返回0
int main()
{
	int fd1 = open("hello.txt", O_RDWR);
	perror("fd1");
	char buf[1024];
	int count = 0;
	if((count = read(fd1, buf, 100)) != 0){
		buf[count] = '\0';
		printf("buf : %s", buf);
	}
	return 0;
}

