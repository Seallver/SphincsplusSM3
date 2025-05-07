#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "params.h"

#include "globals.h"
#include "logic.h"

BIGNUM* prime;
pthread_barrier_t barrier;
int threshold[SPX_D];


#define SPX_MLEN 256
#define NUMBER_OF_THREADS PLAYERS + 1 //线程数目，注意需要加上TTP

// 比较函数（升序排序）
int compare_asc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b); // 升序
}

//打印签名内容
void print_sigma(const unsigned char *sigma) {
    printf("SIGMA:\n");
    printf("{\n");
    printf("\tR:\t");
    for (int i = 0; i < SPX_N; i++) {
        if (i >= 0 && i % 64 == 0) printf("\t");
        printf("%02x", sigma[i]);
    }
    printf("\n");
    sigma += SPX_N;

    printf("\tSig_FORS:\t");
    for (int i = 0; i < SPX_FORS_BYTES; i++) {
        if (i > 0 && i % 64 == 0) printf("\n\t\t\t");
        printf("%02x", sigma[i]);
    }
    printf("\n");
    sigma += SPX_FORS_BYTES;

    printf("\tSig_WOTS:\n");
    for (int i = 0; i < SPX_D; i++) {
        printf("\t\t[%d] ", i);
        for (int j = 0; j < SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N; j++) {
            if (j > 0 && j % 64 == 0) printf("\n\t\t\t");
            if (j == 0)printf("\t");
            printf("%02x", sigma[j]);
        }
        printf("\n");
        sigma += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    }
    printf("\n");
    printf("}\n");
    printf("SIGMA bytes: %d\n", SPX_BYTES);
    printf("SIGMA size: %.2f KB\n", SPX_BYTES / 1024.0);
    
}

int main(void)
{
    //声明变量
    ThreadSafeQueue channel[NUMBER_OF_THREADS]; //信道
    thread_ctx ctx[NUMBER_OF_THREADS]; //线程上下文
    pthread_t threads[NUMBER_OF_THREADS]; //线程
    int ret; //线程返回值
    unsigned char* M = malloc(SPX_MLEN); //消息明文

    //生成安全素数
    prime = BN_new();
    init_crypto_params(prime);
    printf("lagrange prime = %s\n", BN_bn2dec(prime));
    //初始化屏障
    pthread_barrier_init(&barrier, NULL, PLAYERS);
    
    printf("Threshold Testing (n = %d, t = %d)\n", PLAYERS, THRESHOLD);


    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        ThreadSafeQueue_Init(&channel[i], (unsigned int) i);
    }

    //每个线程的ctx初始化
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        if (init_ctx(&ctx[i], i, SPX_MLEN ,&channel[i], channel) != 0) {
            printf("Failed to initialize context\n");
            return -1;
        }
    }

    //密钥生成
    printf("KEYGEN: start...\n");
    pthread_create(&threads[0], NULL, keygen_TTP_logic , &ctx[0]);
    for (int i = 1;i < NUMBER_OF_THREADS;i++) {
        pthread_create(&threads[i], NULL, keygen_player_logic , &ctx[i]);
    }
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        pthread_join(threads[i], NULL);
    }
    printf("KEYGEN: successful\n");

    //修改线程屏障
    pthread_barrier_destroy(&barrier);
    pthread_barrier_init(&barrier, NULL, THRESHOLD);

    //随机生成明文并分配给参与方上下文
    randombytes(M, SPX_MLEN);
    for (int i = 0;i < NUMBER_OF_THREADS;i++) {
        if (input_m(&ctx[i], M) != 0) {
            printf("Failed to input m into context\n");
            return -1;
        }
    }
    printf("message:\t\t");
    for (int i = 0;i < SPX_MLEN;i++) {
        printf("%02x", M[i]);
    }
    printf("\n");


    //随机选取门限参与方
    srand(time(NULL));
    for (int i = 0;i < PLAYERS;i++) {
        threshold[i] = i + 1;
    }
    for (int i = PLAYERS - 1; i > 0; --i) {
        int j = rand() % (i + 1); 
        int temp = threshold[i];
        threshold[i] = threshold[j];
        threshold[j] = temp;
    }
    for (int i = 0;i < THRESHOLD;i++) {
        qsort(threshold, THRESHOLD, sizeof(int), compare_asc);
    }
    if (IS_PRINT) {
        printf("Threshold players:\t");
        for (int i = 0;i < THRESHOLD;i++) {
            printf("%d ", threshold[i]);
        }
        printf("\n");
    }

    //签名
    printf("SIGN: start...\n");
    for (int i = 0;i < THRESHOLD;i++) {
        int tid = threshold[i];
        pthread_create(&threads[tid], NULL, sign_player_logic, &ctx[tid]);
    }
    pthread_create(&threads[0], NULL, sign_TTP_logic, &ctx[0]);
    for (int i = 0;i < THRESHOLD;i++) {
        int tid = threshold[i];
        pthread_join(threads[tid], NULL);
    }
    pthread_join(threads[0], NULL);
    printf("SIGN: successful\n");

    unsigned char sigma[ctx[threshold[0]].smlen];
    memcpy(sigma, (&ctx[threshold[0]]) -> sm - ctx[threshold[0]].smlen, ctx[threshold[0]].smlen);
    print_sigma(sigma);

    //验证
    printf("VERIFY: start...\n");
    for (int i = 0;i < 1;i++) {
        int tid = threshold[i];
        pthread_create(&threads[tid], NULL, vrfy_player_logic, &ctx[tid]);
    }
    for (int i = 0;i < 1;i++) {
        int tid = threshold[i];
        pthread_join(threads[tid], &ret);
        if (ret){
            printf("VERIFY: failed\n");
            return -1;
        }
    }
    printf("VERIFY: successful\n");

    return 0;
}