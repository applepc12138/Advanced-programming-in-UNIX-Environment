/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月01日 星期二 13时11分22秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

sem_t sem;

int main()
{
	sem_init(&sem, 0, 0);
	int count = 0;
	sem_getvalue(&sem, &count);
	printf("value of semaphor is %d\n", count);
	//当信号量为初始值时再使用sem_post()时信号量值不再发生变化
	sem_post(&sem);
	sem_getvalue(&sem, &count);
	printf("after post value of semaphor is %d\n", count);
	sem_wait(&sem);
	sem_getvalue(&sem, &count);
	printf("after sem_wait value of semaphore is %d\n", count);
	return 0;
}


