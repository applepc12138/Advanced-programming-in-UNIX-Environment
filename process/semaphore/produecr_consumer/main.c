/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月03日 星期四 19时59分26秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define MAX 5

sem_t empty_sem, full_sem;
pthread_mutex_t mutex;

int queue[MAX];
int i = 0;

void *producer(void *arg)
{
	while(1){
		sem_wait(&empty_sem);
		pthread_mutex_lock(&mutex);
		queue[i] = rand() % 100 + 1;
		printf("thread=%ld\tproduce queue[%d]=%d\n", pthread_self(), i, queue[i]);
		++i;
		i = i % MAX;
		pthread_mutex_unlock(&mutex);
		sem_post(&full_sem);
		usleep(327000);
	}
	return NULL;
}

void *consumer(void *arg)
{
	while(1){
		sem_wait(&full_sem);
		pthread_mutex_lock(&mutex);
		i = (i == 0 ? MAX : i);
		printf("thread=%ld\tconsume queue[%d]=%d\n", pthread_self(), i - 1, queue[i - 1]);
		queue[--i] = 0;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty_sem);
		usleep(91000);
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	sem_init(&empty_sem, 0, 5);
	sem_init(&full_sem, 0, 0);
	pthread_mutex_init(&mutex, NULL);

	pthread_t tid[4];
	for(int i = 0; i< 3; ++i)
		pthread_create(&tid[i], NULL, producer, NULL);
	pthread_create(&tid[3], NULL, consumer, NULL);

	for(int i = 0; i < 4; ++i)
		pthread_join(tid[i], NULL);
	sem_destroy(&empty_sem);
	sem_destroy(&full_sem);
	return 0;
}

