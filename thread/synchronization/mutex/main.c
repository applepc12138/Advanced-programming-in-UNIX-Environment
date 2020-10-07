/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月31日 星期一 09时38分28秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
//int pthread_mutex_destroy(pthread_mutex_t *mutex);
//int pthread_mutex_lock(pthread_mutex_t *mutex);
//int pthread_mutex_trylock(pthread_mutex_t *mutex);
//int pthread_mutex_unlock(pthread_mutex_t *mutex);

int i = 0;
pthread_mutex_t mutex;

void *thread_handler(void *arg)
{
	pthread_mutex_lock(&mutex);
	printf("sub thread i = %d\n", i);
	pthread_mutex_unlock(&mutex);
	while(i < 100){
		pthread_mutex_lock(&mutex);
		++i;
		printf("sub thread i = %d\n", i);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);
	if(pthread_create(&tid, NULL, thread_handler, NULL) != 0)
		return 0;
	pthread_mutex_lock(&mutex);
	printf("main thread i = %d\n", i);
	pthread_mutex_unlock(&mutex);
	
	while(i < 100){
		pthread_mutex_lock(&mutex);
		++i;
		printf("main thread i = %d\n", i);
		pthread_mutex_unlock(&mutex);
	}
	pthread_join(tid, NULL);
	return 0;
}
