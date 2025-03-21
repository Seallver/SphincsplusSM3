#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 定义队列节点
typedef struct Node {
    void* data;          // 数据指针
    struct Node* next;   // 指向下一个节点
} Node;

// 定义线程安全队列
typedef struct {
    Node* head;           // 队列头
    Node* tail;           // 队列尾
    pthread_mutex_t lock; // 互斥锁
    pthread_cond_t cond;  // 条件变量
} ThreadSafeQueue;

// 初始化队列
void ThreadSafeQueue_Init(ThreadSafeQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->lock, NULL); // 初始化互斥锁
    pthread_cond_init(&queue->cond, NULL);  // 初始化条件变量
}

// 销毁队列
void ThreadSafeQueue_Destroy(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁
    Node* current = queue->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current); // 释放节点内存
        current = next;
    }
    pthread_mutex_unlock(&queue->lock); // 解锁
    pthread_mutex_destroy(&queue->lock); // 销毁互斥锁
    pthread_cond_destroy(&queue->cond);  // 销毁条件变量
}

// 向队列中添加元素
void ThreadSafeQueue_Push(ThreadSafeQueue* queue, void* data) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // 创建新节点
    newNode->data = data;
    newNode->next = NULL;

    pthread_mutex_lock(&queue->lock); // 加锁

    if (queue->tail == NULL) { // 如果队列为空
        queue->head = newNode;
        queue->tail = newNode;
    } else { // 如果队列不为空
        queue->tail->next = newNode;
        queue->tail = newNode;
    }

    pthread_cond_signal(&queue->cond); // 通知等待的线程
    pthread_mutex_unlock(&queue->lock); // 解锁
}

// 从队列中取出元素
void* ThreadSafeQueue_Pop(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁

    while (queue->head == NULL) { // 如果队列为空，等待
        pthread_cond_wait(&queue->cond, &queue->lock);
    }

    Node* node = queue->head; // 获取队列头节点
    void* data = node->data;  // 获取数据

    if (queue->head == queue->tail) { // 如果队列只有一个节点
        queue->head = NULL;
        queue->tail = NULL;
    } else { // 如果队列有多个节点
        queue->head = node->next;
    }

    free(node); // 释放节点内存
    pthread_mutex_unlock(&queue->lock); // 解锁
    return data;
}

// 检查队列是否为空
int ThreadSafeQueue_Empty(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->lock); // 加锁
    int isEmpty = (queue->head == NULL); // 判断队列是否为空
    pthread_mutex_unlock(&queue->lock); // 解锁
    return isEmpty;
}

// // 生产者线程函数
// void* producer(void* arg) {
//     ThreadSafeQueue* queue = (ThreadSafeQueue*)arg;
//     for (int i = 0; i < 10; i++) {
//         int* data = (int*)malloc(sizeof(int));
//         *data = i;
//         ThreadSafeQueue_Push(queue, data);
//         printf("Produced: %d\n", i);
//     }
//     return NULL;
// }

// // 消费者线程函数
// void* consumer(void* arg) {
//     ThreadSafeQueue* queue = (ThreadSafeQueue*)arg;
//     for (int i = 0; i < 10; i++) {
//         int* data = (int*)ThreadSafeQueue_Pop(queue);
//         printf("Consumed: %d\n", *data);
//         free(data); // 释放数据内存
//     }
//     return NULL;
// }

// int main() {
//     ThreadSafeQueue queue;
//     ThreadSafeQueue_Init(&queue); // 初始化队列

//     pthread_t producerThread, consumerThread;
//     pthread_create(&producerThread, NULL, producer, &queue); // 创建生产者线程
//     pthread_create(&consumerThread, NULL, consumer, &queue); // 创建消费者线程

//     pthread_join(producerThread, NULL); // 等待生产者线程结束
//     pthread_join(consumerThread, NULL); // 等待消费者线程结束

//     ThreadSafeQueue_Destroy(&queue); // 销毁队列
//     return 0;
// }