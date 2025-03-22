#include "ThreadSafeQueue.h"

// 初始化队列
void ThreadSafeQueue_Init(ThreadSafeQueue* queue, unsigned int tid) {
    queue->tid = tid;
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->lock, NULL); // 初始化互斥锁
    pthread_cond_init(&queue->cond, NULL);  // 初始化条件变量
}

// 销毁队列
void ThreadSafeQueue_Destroy(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁
    Msg* current = queue->head;
    while (current != NULL) {
        Msg* next = current->next;
        free(current); // 释放节点内存
        current = next;
    }
    pthread_mutex_unlock(&queue->lock); // 解锁
    pthread_mutex_destroy(&queue->lock); // 销毁互斥锁
    pthread_cond_destroy(&queue->cond);  // 销毁条件变量
}

// 向队列中添加元素
void ThreadSafeQueue_Push(ThreadSafeQueue* queue, Msg* newMsg) {
    pthread_mutex_lock(&queue->lock); // 加锁
    if (queue->tail == NULL) { // 如果队列为空
        queue->head = newMsg;
        queue->tail = newMsg;
    } else { // 如果队列不为空
        queue->tail->next = newMsg;
        queue->tail = newMsg;
    }

    pthread_cond_signal(&queue->cond); // 通知等待的线程
    pthread_mutex_unlock(&queue->lock); // 解锁
}

// 从队列中取出元素
Msg* ThreadSafeQueue_Pop(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁

    while (queue->head == NULL) { // 如果队列为空，等待
        pthread_cond_wait(&queue->cond, &queue->lock);
    }
    Msg* msg = queue->head; // 获取队列头节点

    if (queue->head == queue->tail) { // 如果队列只有一个节点
        queue->head = NULL;
        queue->tail = NULL;
    } else { // 如果队列有多个节点
        queue->head = msg->next;
    }
    
    pthread_mutex_unlock(&queue->lock); // 解锁
    
    return msg;
}

// 检查队列是否为空
int ThreadSafeQueue_Empty(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁
    int isEmpty = (queue->head == NULL); // 判断队列是否为空
    pthread_mutex_unlock(&queue->lock); // 解锁
    return isEmpty;
}
