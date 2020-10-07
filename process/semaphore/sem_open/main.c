/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月28日 星期五 14时15分23秒
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//打开一个已有的或创建一个新的信号量
//sem_t *sem_open(const char *name, int flag, .../*mode_t mode, unsigned int value*/);
//int sem_close(sem_t *sem);
//进程未手动关闭的信号量内核将自动关闭
//int sem_unlink(const char*name);
//sem_unlink()函数删除信号量的名字,删除将延迟到最后一个打开的信号量关闭

int main()
{
	//创建一个信号量
	sem_t *semptr = sem_open("SEM_", O_CREAT | O_EXCL, S_IRWXU, 1);
	if(semptr == SEM_FAILED)
		perror("create semaphore fail");
	sem_close(semptr);
	//删除信号量
	sem_unlink("SEM_");
	return 0;
}
