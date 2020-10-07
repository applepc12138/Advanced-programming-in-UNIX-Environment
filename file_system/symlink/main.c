/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月24日 星期一 11时50分27秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	creat("hello.txt", S_IRUSR | S_IWUSR);
	//创建符号链接
	symlink("hello.txt", "symlink.txt");

	//读取符号链接文件中的内容
	//返回读取字节数
	char buf[512];
	int count = readlink("symlink.txt", buf, 512);
	buf[count] = '\0';
	printf("sysmlink.txt : %s\n", buf);
	
	return 0;
}
