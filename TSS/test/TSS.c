#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "TSS_api.h"
#include "params.h"
#include "randombytes.h"
#include "ThreadSafeQueue.h"
#include "VSS.h"
#include "communication.h"

#define SPX_MLEN 32

//定义线程上下文结构体
typedef struct {
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    unsigned char *m;
    unsigned char *sm;
    unsigned char *mout;
    unsigned long long smlen;
    unsigned long long mlen;
    unsigned int tid;
    ThreadSafeQueue *self_channel;
    ThreadSafeQueue *public_channel_list;

} thread_ctx;

//创建屏障
pthread_barrier_t barrier;


//初始化参与方的ctx
int init_ctx(thread_ctx* ctx, int tid, unsigned char* M, ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list) {
    ctx->m = malloc(SPX_MLEN);
    memcpy(ctx->m, M, SPX_MLEN);
    ctx->sm = malloc(SPX_BYTES + SPX_MLEN);
    ctx->mout = malloc(SPX_BYTES + SPX_MLEN);
    ctx->tid = tid;
    ctx->self_channel = channel;
    ctx->public_channel_list = public_channel_list;

    if (ctx->m == NULL || ctx->sm == NULL || ctx->mout == NULL) {
        free(ctx->m);
        free(ctx->sm);
        free(ctx->mout);
        return -1; // 初始化失败
    }

    return 0; // 初始化成功
}

//线程逻辑
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

    //从可信第三方接收p和g
    uint64_t *p = malloc(sizeof(uint64_t));
    uint64_t *g = malloc(sizeof(uint64_t));
    int* party_id = malloc(sizeof(int));
    Recv_Msg(ctx->self_channel, party_id, &p);
    Recv_Msg(ctx->self_channel, party_id, &g);
    // printf("Thread %d: Received p = %llu, g = %llu\n", tid, *p, *g);

    //到达屏障阻塞，等待其他线程接收完毕
    pthread_barrier_wait(&barrier);

    //初始化VSS上下文
    VSS_ctx vss_ctx;
    VSS_init(&vss_ctx, *p, *g);
    printf("Thread %d: VSS initialized\n", tid);

    return 0;
}

int TTP_logic(thread_ctx* ctx) {
    BIGNUM* p = BN_new();
    BIGNUM* g = BN_new();
    init_crypto_params(p, g);
    printf("TTP: p = %s\n", BN_bn2dec(p));
    printf("TTP: g = %s\n", BN_bn2dec(g));
    void* ptrp = (void*)p;
    void* ptrg = (void*)g;
    Send_Msg(ctx->public_channel_list, 0, -1, ptrp);
    Send_Msg(ctx->public_channel_list, 0, -1, ptrg);
    //到达屏障阻塞，等待其他线程接收完毕
    pthread_barrier_wait(&barrier);

    return 0;
}

int main(void)
{
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

    //各参与方开始进行SPHINCS+
    for (int i = 1;i < NUMBER_OF_THREADS;i++) {
        pthread_create(&threads[i], NULL, thread_logic , &ctx[i]);
    }
    //TTP等待分发公钥
    pthread_create(&threads[0], NULL, TTP_logic , &ctx[0]);

    
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