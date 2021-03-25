/*************************************************************************
	> File Name: thread_pool.h
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月21日 星期三 18时32分56秒
 ************************************************************************/

#include <pthread.h>

//任务队列
typedef struct{
	void *(*func)(void *);
	void *arg;
}threadpool_task;

//线程池对象
typedef struct{
	pthread_mutex_t lock;
	pthread_mutex_t work_lock;
	pthread_cond_t taskqueue_empty;
	pthread_cond_t taskqueue_full;
	threadpool_task *taskqueue;		//任务队列，用数组表示
	pthread_t manage_thread;		//管理线程id

	pthread_t *threads_ID;			//保存线程id的动态数组
	int min_thread_num;
	int max_thread_num;
	int cur_thread_num;
	int cur_work_num;
	int wait_exit_thread_num;		//
	int taskqueue_front;
	int taskqueue_tail;
	int taskqueue_cur_size;
	int taskqueue_max_size;
	int shutdown;					//是否关闭线程池 
}threadpool; 


threadpool* threadpool_create(int , int , int);

void threadpool_addtask(threadpool *pool,
		void *(*func)(void *arg),
		void *arg);

void *threadpool_runtask(void *pool);

void *threadpool_manage(void *pool);

void threadpool_destroy(void *pool);

void threadpool_free(void *pool);

int is_thread_alive(pthread_t tid);
