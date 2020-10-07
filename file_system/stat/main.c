/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 18时34分17秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	struct stat buf;
	stat("/home/ggboypc12138/c++_project/insertSort/insertSort.cpp", &buf);
	printf("inode num %ld \n", buf.st_ino);
	printf("hard links num %ld\n", buf.st_nlink);
	printf("usr ID %d\n", buf.st_uid);
	printf("group ID %d\n", buf.st_gid);
	printf("file size %ld\n", buf.st_size);
	printf("block num %ld\n", buf.st_blksize);
	return 0;
}

