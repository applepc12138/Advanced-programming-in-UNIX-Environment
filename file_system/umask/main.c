/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 21时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
//umask()只用于本进程创建新文件时设置权限位
//mode_t umask(mode_t cmask);
//返回值：之前创建的屏蔽字
int main()
{
	umask(0);
	if(creat("hello.txt", RWRWRW) < 0){
		perror("creat file fail");
		exit(1);
	}
	umask(S_IWOTH | S_IROTH);
	if(creat("world.txt", RWRWRW) < 0){
		perror("creat file fail");
		exit(1);
	}
	
	return 0;
}

