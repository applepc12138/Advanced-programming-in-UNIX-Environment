/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 13时05分57秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//int chdir(const char *path);
//
//char *getcwd(char *buf, size_t size);

int main()
{
	if(chdir("~/c_project/unix环境高级编程/file_system/dir/chdir_getcwd") == 0){
		perror("change dir fail");
		return 0;
	}
	char buf[255];
	printf("buf size is %ld\n", sizeof(buf));
	getcwd(buf, sizeof(buf));
	printf("present work dir is:\n %s\n", buf);
	return 0;
}

