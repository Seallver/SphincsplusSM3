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
    printf("Thread %d: Waiting at barrier\n", tid);
    int rett = pthread_barrier_wait(&barrier);
    if (rett == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Thread %d is the serial thread\n", tid);
    }
    printf("Thread %d: Passed barrier\n", tid);


    // if (IS_PRINT)
    //     printf("Generating keypair.. \n");
    
    // if (tss_crypto_sign_keypair(pk, sk)) {
    //     printf("failed!\n");
    //     return -1;
    // }
    // if (IS_PRINT)
    //     printf("successful.\n");
    

    // tss_crypto_sign(sm, &smlen, m, SPX_MLEN, sk);

    // if (smlen != SPX_BYTES + SPX_MLEN) {
    //     printf("  X smlen incorrect [%llu != %u]!\n",
    //             smlen, SPX_BYTES);
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    smlen as expected [%llu].\n", smlen);
    // }

    // /* Test if signature is valid. */
    // if (tss_crypto_sign_open(mout, &mlen, sm, smlen, pk)) {
    //     printf("  X verification failed!\n");
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    verification succeeded.\n");
    // }

    // /* Test if the correct message was recovered. */
    // if (mlen != SPX_MLEN) {
    //     printf("  X mlen incorrect [%llu != %u]!\n", mlen, SPX_MLEN);
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    mlen as expected [%llu].\n", mlen);
    // }
    // if (memcmp(m, mout, SPX_MLEN)) {
    //     printf("  X output message incorrect!\n");
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    output message as expected.\n");
    // }

    // /* Test if signature is valid when validating in-place. */
    // if (tss_crypto_sign_open(sm, &mlen, sm, smlen, pk)) {
    //     printf("  X in-place verification failed!\n");
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    in-place verification succeeded.\n");
    // }

    // /* Test if flipping bits invalidates the signature (it should). */

    // /* Flip the first bit of the message. Should invalidate. */
    // sm[smlen - 1] ^= 1;
    // if (!tss_crypto_sign_open(mout, &mlen, sm, smlen, pk)) {
    //     printf("  X flipping a bit of m DID NOT invalidate signature!\n");
    //     ret = -1;
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("    flipping a bit of m invalidates signature.\n");
    // }
    // sm[smlen - 1] ^= 1;

    return 0;
}

int TTP_logic(thread_ctx* ctx) {
    uint64_t p,  g;
    init_crypto_params(&p, &g);
    printf("TTP: p = %llu, g = %llu\n", p, g);
    Send_Msg(ctx->public_channel_list, 0, -1, &p);
    Send_Msg(ctx->public_channel_list, 0, -1, &g);

    //到达屏障阻塞，等待其他线程接收完毕
    printf("Thread 0: Waiting at barrier\n");
    int rett = pthread_barrier_wait(&barrier);
    if (rett == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Thread 0 is the serial thread\n");
    }
    printf("Thread 0: Passed barrier\n");

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