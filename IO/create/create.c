/*************************************************************************
	> File Name: create.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月22日 星期六 21时02分03秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>

//int creat(const char *path, mode_t mode);
//等价于int open(path, OWRONLY | O_CREAT | O_TRUNC, mode);

int main()
{
	//create()函数只能以只写方式创建文件
	int fd1 = creat("hellofile.txt", S_IRWXU);
	perror("fd1:");
	//如若不显式关闭文件，进程终止时内核自动关闭进程打开的文件
	close(fd1);
	return 0;
}
