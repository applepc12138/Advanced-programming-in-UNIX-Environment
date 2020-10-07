/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 22时53分09秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main()
{
	umask(0);
	if(creat("hello.txt", RWRWRW) < 0)
		perror("create file fail");
	chmod("hello.txt", S_IRUSR | S_IRGRP | S_IROTH);
	return 0;
}
