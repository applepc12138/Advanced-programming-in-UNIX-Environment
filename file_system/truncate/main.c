/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月24日 星期一 09时20分10秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd = creat("hello.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if(fd < 0){
		perror("create file fail");
		exit(1);
	}
	struct stat info;
	fstat(fd, &info);
	printf("hello.txt size is %ld \n", info.st_size);
	char buf[] = "test and change file size";
	int count = write(fd, buf, strlen(buf));	
	printf("count is %d \n", count);
	fstat(fd, &info);
	printf("hello.txt size is %ld \n", info.st_size);
	ftruncate(fd, 100000);
	fstat(fd, &info);
	printf("hello.txt size is %ld \n", info.st_size);
	ftruncate(fd, 5);
	fstat(fd, &info);
	printf("hello.txt size is %ld \n", info.st_size);

	return 0;
}

