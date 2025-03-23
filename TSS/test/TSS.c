#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "TSS_api.h"
#include "params.h"
#include "randombytes.h"
#include "ThreadSafeQueue.h"
#include "round.h"
#include "VSS.h"
#include "tContext.h"



//声明全局变量p和g，代表所有参与方均已知
BIGNUM* prime;
BIGNUM* generator;

//定义线程上下文结构体


//创建屏障
pthread_barrier_t barrier;

//参与方线程逻辑
int thread_logic(thread_ctx* ctx) {
    
    int ret = 0;
    unsigned int tid = ctx->tid;

    printf("Thread %u start\n", tid);
    /* Make stdout buffer more responsive. */
    setbuf(stdout, NULL);
    
    unsigned char *pk = &(ctx->pk);
    unsigned char *sk = &(ctx->sk);
    unsigned char *m = ctx->m;
    unsigned char *sm = ctx->sm;
    unsigned char *mout = ctx->mout;
    unsigned long long smlen;
    unsigned long long mlen;

    keygen_round_player(ctx, prime, generator);

    //到达屏障阻塞，等待其他线程接收完毕
    printf("Thread %d: finished\n", tid);
    pthread_barrier_wait(&barrier);

    return 0;
}

//可信第三方线程逻辑
int TTP_logic(thread_ctx* ctx) {
    printf("Thread 0 start\n");

    keygen_round_ttp(ctx, prime, generator);

    //到达屏障阻塞，等待其他线程接收完毕

    pthread_barrier_wait(&barrier);

    return 0;
}

int main(void)
{
    
    //生成安全素数和原根
    prime = BN_new();
    generator = BN_new();
    init_crypto_params(prime, generator);
    printf("prime = %s\n", BN_bn2hex(prime));
    printf("generator = %s\n", BN_bn2hex(generator));

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, NUMBER_OF_THREADS);

    //随机生成明文
    unsigned char* M = malloc(SPX_MLEN);
    randombytes(M, SPX_MLEN);
    
    printf("Threshold Testing (n = %d, t = %d)\n", NUMBER_OF_THREADS, THRESHOLD);

    //创建信道
    ThreadSafeQueue channel[NUMBER_OF_THREADS];
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        ThreadSafeQueue_Init(&channel[i], (unsigned int) i);
    }


    //声明每个线程的ctx并初始化
    thread_ctx ctx[NUMBER_OF_THREADS];
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        if (init_ctx(&ctx[i], i, M, &channel[i], channel) != 0) {
            printf("Failed to initialize context\n");
            return -1;
        }
    }


    //声明线程（参与方与可信第三方）
    pthread_t threads[NUMBER_OF_THREADS];

    //TTP开始进行SPHINCS+
    pthread_create(&threads[0], NULL, TTP_logic , &ctx[0]);

    //各参与方开始进行SPHINCS+
    for (int i = 1;i < NUMBER_OF_THREADS;i++) {
        pthread_create(&threads[i], NULL, thread_logic , &ctx[i]);
    }


    
    //检查各个线程工作是否成功
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        int res = 0;
        pthread_join(threads[i], &res);
        if (res) {
            printf("Thread %d Failed to signature\n", i);
            return -1;
        }
    }
}