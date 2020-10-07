/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月28日 星期五 15时41分35秒
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

//信号量有命名和未命名信号量两种形式,sem_init()用于创建未命名信号量
//int sem_init(sem_t *sem, int pshared, unsigned int value);
//pshared表明是否在多个进程中使用信号量。如果是,设置其为非零值
//若要在两个及以上进程共享信号量，需确保sem参数指向多个进程共享的内存范围

struct slock{
	sem_t *semp;
	char name[1024];
};

int main()
{

	return 0;
}

struct slock *s_alloc()
{
	struct slock *sp;
	static int cnt;
	if((sp = (struct slock *)malloc(sizeof(struct slock))) == NULL)
		return NULL;
	do{
		snprintf(sp->name, sizeof(sp->name), "/%ld.%d", (long)getpid(), cnt++);
		sp->semp = sem_open(sp->name, O_CREAT | O_EXCL, S_IRWXU, 1);
	}while((sp->semp == SEM_FAILED) && (errno == EEXIST));
	if(sp->semp == SEM_FAILED){
		free(sp);
		return NULL;
	}
	sem_unlink(sp->name);
	return sp;
}

void s_free(struct slock *sp)
{
	sem_close(sp->semp);
	free(sp);
}

int s_lock(struct slock *sp)
{
	return (sem_wait(sp->semp));
}

int s_trylock(struct slock *sp)
{
	return (sem_trywait(sp->semp));
}

int s_unlock(struct slock *sp)
{
	return (sem_post(sp->semp));
}
