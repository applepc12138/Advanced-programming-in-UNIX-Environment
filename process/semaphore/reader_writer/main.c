/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月04日 星期五 10时03分10秒
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define buf_size 4096

sem_t count_sem, write_sem, data_sem;
sem_t datasize_sem;

char buf[buf_size];
int datasize = 0;
int readcount = 0;
int flag = 0;

void *reader(void *arg)
{
	while(1){
		sem_wait(&count_sem);
		if(readcount == 0){
			sem_wait(&write_sem);
		}
		++readcount;
		sem_post(&count_sem);

		sem_wait(&datasize_sem);//同一时间只有一个读者线程能访问此临界区		
		//buf中无数据则阻塞自身
		if(datasize == 0){
			flag = 1;//提示写者线程缓冲区无数据
			sem_post(&write_sem);
			sem_wait(&data_sem);//同步
			sem_wait(&write_sem);
		}
		sem_post(&datasize_sem);

		//读数据
		printf("thread id=%ld\tread data: %s\n", pthread_self(), buf);

		sem_wait(&count_sem);
		--readcount;
		if(readcount == 0){
			sem_post(&write_sem);
		}
		sem_post(&count_sem);
		sleep(2);
	}
	return NULL;
}

void *writer(void *arg)
{
	char tmp[] = "hello linux";
	int i = 0;
	while(1){
		sem_wait(&write_sem);
		//写数据
		buf[datasize++] = tmp[i++];
		//buf[datasize] = '\0';
		//printf("thread id=%ld\twrite %c\n", pthread_self(), buf[datasize - 1]);
		printf("write thread : bufsize = %d\t", datasize);
		for(int i = 0; i < datasize; ++i)
			putchar(buf[i]);
		putchar('\n');

		sem_post(&write_sem);
		if(flag == 1){
			sem_post(&data_sem);
			flag = 0;
		}
		i = i % sizeof(tmp);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	sem_init(&count_sem, 0, 1);
	sem_init(&write_sem, 0, 1);
	sem_init(&data_sem, 0, 0);
	sem_init(&datasize_sem, 0, 1);
	
	pthread_t tid[3];
	for(int i = 0; i < 2; ++i)
		pthread_create(&tid[i], NULL, reader, NULL);
	pthread_create(&tid[2], NULL, writer, NULL);
	
	for(int i = 0; i < 3; ++i)
		pthread_join(tid[i], NULL);

	return 0;
}
