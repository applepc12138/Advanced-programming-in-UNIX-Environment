/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月31日 星期一 13时23分59秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int count = 0;

//似乎信号只能被进程接收
void sig_handler(int signo)
{
	printf("thread %d exit\n", (int)pthread_self());
	pthread_exit(NULL);
}

void *thread_read(void *arg)
{
	signal(SIGALRM, sig_handler);
	while(1){
		pthread_rwlock_rdlock(&rwlock);
		printf("thread id=%d\tcount=%d\n", pthread_self(), count);
		pthread_rwlock_unlock(&rwlock);
		sleep(2);
	}
}

void *thread_write(void *arg)
{
	signal(SIGALRM, sig_handler);
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		++count;
		pthread_rwlock_unlock(&rwlock);
		sleep(2);
	}
}

int main(int argc, char *argv[])
{
	printf("mian thread id = %d\n", pthread_self());
	pthread_t tid[8];
	pthread_rwlock_init(&rwlock, NULL);
	for(int i = 0; i < 5; ++i)
		pthread_create(&tid[0], NULL, thread_read, NULL);
	for(int i = 0; i < 3; ++i)
		pthread_create(&tid[i + 5], NULL, thread_write, NULL);
	alarm(10);
	for(int i = 0; i < 8; ++i){
		pthread_join(tid[i], NULL);
	}
	pthread_rwlock_destroy(&rwlock);
	return 0;
}

