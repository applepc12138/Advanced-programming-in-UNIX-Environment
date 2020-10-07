/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 20时41分28秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//char *getenv(const char *name);
//putenv取形式为name=value的字符串,若name已存在,则删除原先定义
//int putenv(char *str);
//int setenv(const char *name, const char *value, int rewrite);
//删除name定义
//unsetenv(const char *name);

int main()
{
	char *pathptr = getenv("PATH");
	printf("PATH : %s\n", pathptr);

	return 0;
}

