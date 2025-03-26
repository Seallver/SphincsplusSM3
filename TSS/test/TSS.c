#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "logic.h"



BIGNUM* prime;
BIGNUM* generator;
pthread_barrier_t barrier;
int threshold[PLAYERS];

int main(void)
{
    
    //生成安全素数和原根
    prime = BN_new();
    init_crypto_params(prime);
    
    printf("prime = %s\n", BN_bn2dec(prime));

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, PLAYERS);
    
    printf("Threshold Testing (n = %d, t = %d)\n", NUMBER_OF_THREADS, THRESHOLD);

    //创建信道
    ThreadSafeQueue channel[NUMBER_OF_THREADS];
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        ThreadSafeQueue_Init(&channel[i], (unsigned int) i);
    }

    //声明每个线程的ctx并初始化
    thread_ctx ctx[NUMBER_OF_THREADS];
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        if (init_ctx(&ctx[i], i, &channel[i], channel) != 0) {
            printf("Failed to initialize context\n");
            return -1;
        }
    }

    //声明线程（参与方与可信第三方）
    pthread_t threads[NUMBER_OF_THREADS];


    printf("KEYGEN: start...\n");
    //TTP开始keygen
    pthread_create(&threads[0], NULL, keygen_TTP_logic , &ctx[0]);

    //参与方开始keygen
    for (int i = 1;i < NUMBER_OF_THREADS;i++) {
        pthread_create(&threads[i], NULL, keygen_player_logic , &ctx[i]);
    }

    //keygen结束查看各个线程是否成功运行
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        int res = 0;
        pthread_join(threads[i], &res);
        if (res) {
            printf("Thread %d Failed to signature\n", i);
            return -1;
        }
    }

    printf("KEYGEN: successful\n");

    //修改线程屏障
    pthread_barrier_destroy(&barrier);
    pthread_barrier_init(&barrier, NULL, THRESHOLD);

    //随机生成明文并分配给上下文
    unsigned char* M = malloc(SPX_MLEN);
    randombytes(M, SPX_MLEN);
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        if (input_m(&ctx[i], M) != 0) {
            printf("Failed to input m into context\n");
            return -1;
        }
    }


    printf("SIGN: start...\n");

    srand(time(NULL));

    for (int i = 0;i < PLAYERS;i++) {
        threshold[i] = i + 1;
    }
    // Fisher-Yates洗牌算法，将参与方乱序
    for (int i = PLAYERS - 1; i > 0; --i) {
        int j = rand() % (i + 1); 
        int temp = threshold[i];
        threshold[i] = threshold[j];
        threshold[j] = temp;
    }
    
    //从乱序的参与方中选前t个作为门限方进行签名
    for (int i = 0;i < THRESHOLD;i++) {
        int tid = threshold[i];
        pthread_create(&threads[tid], NULL, sign_player_logic, &ctx[tid]);
    }
    //TTP开始签名
    pthread_create(&threads[0], NULL, sign_TTP_logic, &ctx[0]);

    
    //签名结束查看各个线程是否成功运行
    int res = 0;
    pthread_join(threads[0], &res);
    if (res) {
        printf("TTP failed to signature\n");
        return -1;
    }
    for (int i = 0;i < THRESHOLD;i++) {
        int res = 0;
        int tid = threshold[i];
        pthread_join(threads[tid], &res);
        if (res) {
            printf("Thread %d failed to signature\n", i);
            return -1;
        }
    }

    printf("SIGN: successful\n");

}