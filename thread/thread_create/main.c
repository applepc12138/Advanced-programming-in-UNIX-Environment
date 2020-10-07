/*************************************************************************
	> File Name: thread_create.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年07月12日 星期日 15时22分26秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//任意一个线程调用了exit或_exit，则整个进程的所有线程都终止，从main函数return也相当于调用exit

//如果需要只终止某个线程而不终止整个进程，可以有三种方法：
//1.从线程调用函数return。这种方法对主控线程不适用，从main函数return相当于调用exit
//2.一个线程可以调用pthread_cancel终止同一进程中的另一个线程
//3.线程可以调用pthread_exit终止自己

//int pthread_create(pthread_t *tidp, const pthreat_attr_t *attr, void *(*start_rtn)(void *), void *arg);
//tid:新创建的线程id返回至*tidp 
//attr:线程属性
//start_rtn:线程执行函数
//arg:线程执行函数参数
//返回值：成功返回0，否则，返回errno

sem_t sem1, sem2;

void *th_fun(void *arg)
{
	int *p = (int *)arg;
	printf("thread PID %d\n", getpid());
	printf("thread ID %x\n", (unsigned int)pthread_self());
	printf("thread *arg = %d\n", *p);
	sem_post(&sem2);
	sem_wait(&sem1);
	printf("thread *arg = %d\n", *p);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;//保存子线程id
	int n = 10;
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);
	pthread_create(&tid, NULL, th_fun, (void *)&n);
	printf("main thread ID = %x\n", (unsigned int)pthread_self());
	printf("main child thread ID = %x\n", (unsigned int)tid);
	printf("main PID = %d\n", getpid());
	sem_wait(&sem2);
	++n;
	sem_post(&sem1);
	pthread_join(tid, NULL);
	sem_destroy(&sem1);
	sem_destroy(&sem2);

	printf("over\n");

	return 0;
}
