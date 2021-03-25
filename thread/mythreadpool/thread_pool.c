/*************************************************************************
	> File Name: thread_pool.c > Author: ggboypc12138 > Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月21日 星期三 18时30分26秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <signal.h>
#include <errno.h>
#include "thread_pool.h"

//管理线程每20s检查一遍是否
#define DEFAULT_CHECK_TIME 20
//任务队列累积任务，taskqueue_cur_size > MIN_WAIT_TAST_NUM 时需要添加线程到线程池
#define MIN_WAIT_TAST_NUM 10
//每次增加和销毁线程个数
#define DERAULT_ADD_DEL_THREAD 4


//线程池初始化
threadpool *threadpool_create(int min_thr_num, 
		int max_thr_num, int taskqueue_max_size)
{
	threadpool *pool = malloc(sizeof(threadpool));
	pool->min_thread_num = min_thr_num;
	pool->max_thread_num = max_thr_num;
	pool->cur_thread_num = min_thr_num;
	pool->cur_work_num = 0;

	pool->threads_ID = malloc(sizeof(pthread_t) * max_thr_num);
	bzero(pool->threads_ID, max_thr_num * sizeof(pthread_t));

	//任务队列
	pool->taskqueue_front = 0;
	pool->taskqueue_tail = 0;
	pool->taskqueue_cur_size = 0;
	pool->taskqueue_max_size = taskqueue_max_size;
	pool->taskqueue = malloc(taskqueue_max_size * sizeof(threadpool_task));
	bzero(pool->taskqueue, taskqueue_max_size * sizeof(threadpool_task));

	//创建工作线程
	for(int i = 0; i < min_thr_num; ++i){
		pthread_create(&pool->threads_ID[i], NULL, threadpool_runtask, (void *)pool);
	}
	//创建管理线程
	pthread_create(&pool->manage_thread, NULL, threadpool_manage, (void *)pool);

	//初始化锁和条件变量
	pthread_mutex_init(&pool->lock, NULL);
	pthread_mutex_init(&pool->work_lock, NULL);

	pthread_cond_init(&pool->taskqueue_empty, NULL);
	pthread_cond_init(&pool->taskqueue_full, NULL);

	pool->shutdown = 0;

	return pool;
}

//主线程调用,用于向任务队列中添加任务
void threadpool_addtask(threadpool *pool, 
		void *(*func)(void *),
		void *arg)
{
	pthread_mutex_lock(&pool->lock);
	while(pool->taskqueue_cur_size == pool->taskqueue_max_size && pool->shutdown == 0){
		pthread_cond_wait(&pool->taskqueue_empty, &pool->lock);
	}
	if(pool->shutdown == 1){
		pthread_mutex_unlock(&pool->lock);
		return;
	}
	threadpool_task task;
	task.func = func;
	task.arg = arg;
	pool->taskqueue[pool->taskqueue_tail] = task;
	pool->taskqueue_tail = ++pool->taskqueue_tail % pool->taskqueue_max_size;

	pthread_mutex_unlock(&pool->lock);
	pthread_cond_signal(&pool->taskqueue_full);
}

//工作线程运行任务
void *threadpool_runtask(void *pool)
{
	threadpool *pool_ = (threadpool *)pool;
	threadpool_task task; 
	while(1){
		pthread_mutex_lock(&pool_->lock);
		if(pool_->shutdown == 0){
			while(pool_->taskqueue_cur_size == 0){
				pthread_cond_wait(&pool_->taskqueue_full, &pool_->lock);
				//被唤醒后，若线程池被要求关闭
				if(pool_->shutdown == 1){
					pthread_mutex_unlock(&pool_->lock);
					printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
					pthread_exit(NULL);
				}
				//被唤醒后，若wait_exit_thread_num > 0，自行退出
				if(pool_->wait_exit_thread_num > 0){
					--pool_->wait_exit_thread_num;
					pthread_mutex_unlock(&pool_->lock);
					printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
					pthread_exit(NULL);
				}
			}
			//从任务队列中取任务执行
			task = pool_->taskqueue[pool_->taskqueue_front];
			pool_->taskqueue_front = ++pool_->taskqueue_front % pool_->taskqueue_max_size;
			--pool_->taskqueue_cur_size;
			pthread_mutex_unlock(&pool_->lock);
			pthread_cond_signal(&pool_->taskqueue_empty);
		}
		else {
			pthread_mutex_unlock(&pool_->lock);
			return NULL;
		}
		pthread_mutex_lock(&pool_->work_lock);
		++pool_->cur_work_num;
		pthread_mutex_unlock(&pool_->work_lock);

		task.func(task.arg);		
		
		pthread_mutex_lock(&pool_->work_lock);
		--pool_->cur_work_num;
		pthread_mutex_unlock(&pool_->work_lock);
	}
}

//管理线程调用，管理线程主要向线程池中添加线程()和销毁多余线程()
void *threadpool_manage(void *pool)
{
	threadpool *pool_ = (threadpool *)pool;
	while(1){
		
		pthread_mutex_lock(&pool_->lock);
		if(pool_->shutdown == 1){
			pthread_mutex_unlock(&pool_->lock);
			return NULL;
		}
		
		//需要添加线程?
		if(pool_->cur_work_num == pool_->cur_thread_num && pool_->taskqueue_cur_size > 0 
				&& pool_->cur_thread_num < pool_->max_thread_num){
			int cur_num = pool_->cur_thread_num;
			pthread_mutex_unlock(&pool_->lock);
			int accumulate = 0;
			//pool_->taskqueue_max_size,DERAULT_ADD_DEL_THREAD,pool_->max_thread_num值一般是只读的，不需要加锁
			for(int i = 0; i < pool_->taskqueue_max_size 
					&& accumulate < DERAULT_ADD_DEL_THREAD 
					&& (cur_num + accumulate) < pool_->max_thread_num; ++i){
				if(pool_->threads_ID[i] == 0){
					pthread_create(&pool_->threads_ID[i], NULL, threadpool_runtask, pool_);
					++accumulate;
				}
			}
			pthread_mutex_lock(&pool_->lock);
			pool_->cur_thread_num = cur_num + accumulate;
			pthread_mutex_unlock(&pool_->lock);
		}
		else if(pool_->cur_work_num * 2 < pool_->cur_thread_num 
				&& pool_->cur_thread_num > pool_->min_thread_num){//需要销毁多余线程?
			pool_->wait_exit_thread_num = DERAULT_ADD_DEL_THREAD;
			pthread_mutex_unlock(&pool_->lock);
			for(int i = 0; i < DERAULT_ADD_DEL_THREAD; ++i)
				pthread_cond_signal(&pool_->taskqueue_full);
		}
	}
}

//销毁线程池中所有线程并释放线程池相关资源
void threadpool_destroy(void *pool)
{
	threadpool *pool_ = (threadpool *)pool;
	pthread_mutex_lock(&pool_->lock);
	pool_->shutdown = 1;
	int thr_num = pool_->cur_thread_num;
	//必须先销毁管理线程
	pthread_mutex_unlock(&pool_->lock);
	pthread_join(pool_->manage_thread, NULL);
	for(int i = 0; i < thr_num; ++i){
		pthread_cond_broadcast(&pool_->taskqueue_full);
		pthread_join(pool_->threads_ID[i], NULL);//无需加锁，管理线程已退出
	}
	threadpool_free(pool);
}

//释放线程池中相关资源
void threadpool_free(void *pool)
{
	
}

int is_thread_alive(pthread_t tid)
{
	return pthread_kill(tid, 0) == ESRCH ? 0 : 1;
}

