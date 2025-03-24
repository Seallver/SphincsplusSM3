#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "logic.h"


BIGNUM* prime;
BIGNUM* generator;
pthread_barrier_t barrier;


int main(void)
{
    
    //生成安全素数和原根
    prime = BN_new();
    generator = BN_new();
    init_crypto_params(prime, generator);
    
    printf("prime = %s\n", BN_bn2dec(prime));
    printf("generator = %s\n", BN_bn2dec(generator));

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, PLAYERS);

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
        if (init_ctx(&ctx[i], i, M, &channel[i], channel, prime, generator) != 0) {
            printf("Failed to initialize context\n");
            return -1;
        }
    }

    //声明线程（参与方与可信第三方）
    pthread_t threads[NUMBER_OF_THREADS];

    //TTP开始keygen
    pthread_create(&threads[0], NULL, keygen_TTP_logic , &ctx[0]);

    //全部参与方开始keygen
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
}