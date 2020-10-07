/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月30日 星期日 20时20分13秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct foo{
	int a;
	double b;
	char c;
};

struct foo f = {520, 0.25, 'C'};

void *thread_handler(void *arg)
{
	printf("in thread_handler\n");
	pthread_exit((void *)&f);
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_handler, NULL);
	struct foo *ptr;
	pthread_join(tid, (void **)&ptr);
	printf("foo.a = %d\nfoo.b = %f\nfoo.c = %c\n",
			ptr->a, ptr->b, ptr->c);
	return 0;
}

