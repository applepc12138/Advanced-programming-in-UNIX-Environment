/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月31日 星期一 10时39分34秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char*argv[])
{
	struct timespec t;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");
	//int clock_gettime(clockid_t clk_id, struct timespec *tp);
	clock_gettime(CLOCK_REALTIME, &t);
	//size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
	tmp = localtime(&t.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time is %s\n", buf);
	t.tv_sec += 10;
	int err = pthread_mutex_timedlock(&lock, &t);
	clock_gettime(CLOCK_REALTIME, &t);
	tmp = localtime(&t.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time is %s\n", buf);
	if(err == 0)
		printf("mutex locked again\n");
	else
		printf("\n");
	return 0;
}

