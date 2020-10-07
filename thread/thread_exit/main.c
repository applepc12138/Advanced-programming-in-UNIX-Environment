/*************************************************************************
	> File Name: thread_exit.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年07月12日 星期日 19时56分18秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

//void pthread_exit(void *rval_ptr);
//*rval_ptr:线程退出时的返回值(必须保证在线程退出后其值仍可访问)
//int pthread_join(pthread_t thread, void **rval_ptr);
//调用线程将一直阻塞，直到指定的线程调用pthread_exit、从调用线程中返回或者被pthread_cancel()取消

void *th_fn1(void *arg)
{
	printf("thread 1 returning\n");
	return (void *)1;
}

void *th_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	pthread_exit((void *)222);
}

void *th_fn3(void *arg)
{
	while(1){
		printf("thread 3 writing\n");
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	void *tret = NULL;

	pthread_create(&tid, NULL, th_fn1, NULL);
	pthread_join(tid, &tret);
	printf("thread 1 exit code %d\n", (int)tret);
	
	pthread_create(&tid, NULL, th_fn2, NULL);
	pthread_join(tid, &tret);
	printf("thread 2 exit code %d\n", (int)tret);

	pthread_create(&tid, NULL, th_fn3, NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_join(tid, &tret);
	printf("thread 3 exit code %d\n", (int)tret);
	return 0;
}
