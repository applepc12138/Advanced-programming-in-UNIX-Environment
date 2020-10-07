/*************************************************************************
  > File Name: open.c
  > Author: ggboypc12138
  > Mail: lc1030244043@outlook.com 
  > Created Time: 2020年08月22日 星期六 20时35分45秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//int open(const char *pathname, int flags);
//int open(const char *pathname, int flags, mode_t mode);
//返回值：成功返回新分配的文件描述符，出错返回-1并设置errno
int main()
{
	//使用open()文件不存在不会自动创建
	int fd1 = open("hello.txt", O_RDWR);
	if(fd1 == -1)
		perror("fd1 :");

	//使用O_CREAT需要指定文件权限,若不指定权限位则创建一个无权限的文件
	int fd2 = open("world.txt", O_RDWR | O_CREAT);
	if(fd2 == -1)
		perror("fd2 :");

	//S_IWOTH 表示00002权限
	//S_IRUSR 表示00200权限
	int fd3 = open("linux.txt", O_RDWR | O_CREAT, S_IRWXG | S_IRWXO);
	if(fd3 == -1)
		perror("fd3 :");
	return 0;
}

