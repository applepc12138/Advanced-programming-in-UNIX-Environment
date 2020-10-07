/*************************************************************************
	> File Name: write.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月22日 星期六 22时44分10秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//ssize_t write(int fd, const void *buf, size_t count);
//返回值：成功返回写入的字节数，出错返回-1并设置errno
int main()
{
	int fd1 = open("file1.txt", O_RDONLY);
	int fd2 = open("file2.txt", O_WRONLY | O_APPEND);
	if(fd1 != -1 && fd2 != -1){
		char buf[1024];
		int count = read(fd1, buf, 1024);
		buf[count] = '\0';
		//追加写
		write(fd2, buf, count);
	}	
	return 0;
}

