/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 20时01分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//内核使程序执行的唯一方法是调用exec()函数
//进程自愿终止的唯一方法是显式或隐式地调用exit()或_exit()或_Exit()
//apue P161
//用于正常终止一个程序
//exit()会自动调用登记函数,对所有打开的流调用fclose()函数
//void exit(int status);
//void _Exit(int status);
//void _exit(int status);

static void my_exit1(void);
static void my_exit2(void);


int main(int argc, char *argv[])
{
	if(atexit(my_exit1) != 0)
		printf("register my_exit1 fail\n");
	if(atexit(my_exit2) != 0)
		printf("register my_exit2 fail\n");
	if(atexit(my_exit2) != 0)
		printf("register my_exit2 fail\n");

	return 0;
}


static void my_exit1()
{
	printf("first exit handler\n");
}

static void my_exit2()
{
	printf("second exit handler\n");
}
