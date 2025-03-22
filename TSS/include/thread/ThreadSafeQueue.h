#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 定义队列节点
typedef struct{
    int from;            // 数据来源
    int to;              // 数据目的
    void* data;          // 数据指针
    struct Msg* next;   // 指向下一个节点
} Msg;

// 定义线程安全队列
typedef struct {
    unsigned int tid;     // 线程ID
    Msg* head;           // 队列头
    Msg* tail;           // 队列尾
    pthread_mutex_t lock; // 互斥锁
    pthread_cond_t cond;  // 条件变量
} ThreadSafeQueue;

// 初始化队列
void ThreadSafeQueue_Init(ThreadSafeQueue* queue, unsigned int tid);

// 销毁队列
void ThreadSafeQueue_Destroy(ThreadSafeQueue* queue);

// 向队列中添加元素
void ThreadSafeQueue_Push(ThreadSafeQueue* queue, Msg* newMsg);

// 从队列中取出元素
Msg* ThreadSafeQueue_Pop(ThreadSafeQueue* queue);

// 检查队列是否为空
int ThreadSafeQueue_Empty(ThreadSafeQueue* queue);

#endif

