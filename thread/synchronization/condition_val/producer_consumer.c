/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月03日 星期四 09时42分38秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 5

pthread_cond_t full_cond, empty_cond;
pthread_mutex_t mutex;

//临界区资源
int queue[MAX];
int count = 0;
int i = 0;

void *producer(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(count == MAX)
			pthread_cond_wait(&empty_cond, &mutex);
		i = i % MAX;
		queue[i] = rand() % 100 + 1;
		++count;
		printf("thread= %ld\tproduce queue[%d] = %d\t count = %d\n", pthread_self(), i,  queue[i], count);
		++i;
		pthread_cond_signal(&full_cond);
		pthread_mutex_unlock(&mutex);
		//printf("produce %d\n", queue[i]);
		usleep(100);
	}
}

void *consumer(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while(count == 0)
			pthread_cond_wait(&full_cond, &mutex);
		printf("thread= %ld\tconsume queue[%d] = %d\n", pthread_self(),  i - 1, queue[i- 1]);
		if(i == 0){
			i = MAX;
			queue[--i] = 0;
		}
		else
			queue[--i] = 0;
		--count;
		pthread_cond_signal(&empty_cond);
		pthread_mutex_unlock(&mutex);
		//sleep(1);
	}
}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&full_cond, NULL);
	pthread_cond_init(&empty_cond, NULL);

	pthread_t tid[4];
	for(int i = 0; i < 3; ++i)
		pthread_create(&tid[i], NULL, producer, NULL);
	pthread_create(&tid[3], NULL, consumer, NULL);

	for(int i = 0; i < 4; ++i)
		pthread_join(tid[i], NULL);
	return 0;
}

