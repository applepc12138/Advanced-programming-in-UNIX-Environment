/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月24日 星期一 10时51分26秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

//int link(const char *oldpath, const char *newpath);
//int linkat(int efd, const char *oldpath, int nfd, const char *newpath, int flag);

int main()
{
	int fd = open("hello.txt", O_RDWR | O_CREAT, S_IRWXU);
	if(fd < 0){
		perror("create file fail");
		exit(1);
	}
	//创建硬链接
	if( link("hello.txt", "hardlink.txt") < 0){
		perror("create hard link fail");
		exit(1);
	}
	//删除硬链接
	//文件打开时被unlink不会被立即删除
	//对于文件,remove()和unlink()效果相同
	if(unlink("hardlink.txt") < 0){
		perror("delete hard link fail");
		exit(1);
	}
	printf("unlink success");
	return 0;
}
