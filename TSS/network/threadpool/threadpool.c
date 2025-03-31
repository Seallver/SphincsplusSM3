#include "threadpool.h"                                                                                                                                                                                                                       
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

//线程池中创建的线程执行
void *thread_routine(void *arg)
{
    struct timespec abstime;//时间结构体
    int timeout;
    printf("thread %d is starting\n", (int)pthread_self());
    threadpool_t *pool = (threadpool_t *)arg;

    //死循环使线程池中空闲的线程可以复用
    while(1)
    {   
        timeout = 0;

        //访问线程池之前需要加锁
        condition_lock(&pool->ready);

        //空闲
        pool->idle++;

        //任务队列没有任务到来并且没有收到线程池销毁通知, 线程阻塞等待(进入这里面都是空闲线程，等待被唤醒)
        while(pool->first == NULL && !pool->quit)
        {   
            printf("thread %d is waiting\n", (int)pthread_self());

            //获取当前时间，并加上等待时间， 从而设置进程的超时睡眠时间
            clock_gettime(CLOCK_REALTIME, &abstime);  
            abstime.tv_sec += 2;
            int status;
            status = condition_timedwait(&pool->ready, &abstime);  //该函数会解锁，允许其他线程访问，当被唤醒时，加锁
            if(status == ETIMEDOUT)
            {   
                printf("thread %d wait timed out\n", (int)pthread_self());
                timeout = 1;
                break;
            }   
        }   

        pool->idle--;
        if(pool->first != NULL)
        {   
            //取出任务队列最前的任务，移除任务，并执行任务
            task_t *t = pool->first;
            pool->first = t->next;
            //由于任务执行需要消耗时间，先解锁让其他线程访问线程池
            condition_unlock(&pool->ready);
            //执行任务
            t->run(t->arg);
            //执行完任务释放内存
            free(t);
            //重新加锁
            condition_lock(&pool->ready);
        }   

        //退出线程池--销毁当前线程
        if(pool->quit && pool->first == NULL)
        {
            pool->counter--;//当前工作的线程数-1

            //若线程池中没有线程，唤醒等待线程（主线程--销毁线程池的线程）全部任务已经完成
            if(pool->counter == 0)
            {
                condition_signal(&pool->ready);
            }
            condition_unlock(&pool->ready);
            break;
        }
        //超时，说明线程没有任务可以执行, 跳出销毁线程
        if(timeout == 1)
        {
            pool->counter--;//当前工作的线程数-1
            condition_unlock(&pool->ready);
            break;
        }

        condition_unlock(&pool->ready);
    }

    printf("thread %d is exiting\n", (int)pthread_self());
    return NULL;

}


//线程池初始化
void threadpool_init(threadpool_t *pool, int idle_threads, int max_threads)
{

    condition_init(&pool->ready);
    pool->first = NULL;
    pool->last =NULL;
    pool->counter =0;
    pool->idle =0;
    pool->max_threads = max_threads;
    pool->quit =0;

    //创建空闲线程
   int i = 0;
   for(; i < idle_threads; i++)
   {
       pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        pool->counter++;//已有线程数+1
   }

}
//增加一个任务到线程池
int threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
    //产生一个新的任务
    task_t *newtask = (task_t *)malloc(sizeof(task_t));
    newtask->run = run;
    newtask->arg = arg;
    newtask->next=NULL;//新加的任务放在队列尾端

    //线程池的状态被多个线程共享，操作前需要加锁
    condition_lock(&pool->ready);

    if(pool->first == NULL)//第一个任务加入
    {
        pool->first = newtask;
    }        
    else    
    {
        pool->last->next = newtask;
    }
    pool->last = newtask;  //队列尾指向新加入的线程

    //线程池中有线程空闲，唤醒处于等待状态的线程(因为在等待期间会释放互斥锁)
    if(pool->idle > 0)
    {
        condition_signal(&pool->ready);
    }
    //当前线程池中线程个数没有达到设定的最大值，创建一个新的线程
    else if(pool->counter < pool->max_threads)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        pool->counter++;
    }
    else
    {
        condition_unlock(&pool->ready);
        return -1;
    }
    //结束，访问
    condition_unlock(&pool->ready);
    return 0;
}

//线程池销毁
void threadpool_destroy(threadpool_t *pool)
{
    //如果已经调用销毁，直接返回
    if(pool->quit)
    {
        return;
    }
    //加锁
    condition_lock(&pool->ready);                                                                                                                                                                                                             
    //设置销毁标记为1
    pool->quit = 1;
    //线程池中线程个数大于0
  if(pool->counter > 0)
    {
        //对于等待的线程，发送信号唤醒
        if(pool->idle > 0)
        {
            condition_broadcast(&pool->ready);
        }
        //正在执行任务的线程，等待他们结束任务
        while(pool->counter)
        {
            condition_wait(&pool->ready);
        }
    }
    condition_unlock(&pool->ready);
    condition_destroy(&pool->ready);
}