#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "params.h"
#include "globals.h"
#include "logic.h"

#include "save.c"

BIGNUM* prime;
pthread_barrier_t barrier;
int threshold[SPX_D];


#define SPX_MLEN 256

#ifdef TIMETEST
    #define TURNS 100
#else
    #define TURNS 1
#endif

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

int main(void) {
#ifdef TIMETEST
    const char *params_env = getenv("PARAMS_HEAD");
    const char *thash_env = getenv("THASH");
    if (!params_env || !thash_env) {
        fprintf(stderr, "Missing PARAMS or THASH environment variable.\n");
        return 1;
    }
#endif

    double total_time = 0;

    for (int t = 0; t < TURNS; t++) {
        struct timespec start_kg, end_kg, start_sign, end_sign, start_vrfy, end_vrfy;
        double elapsed = 0, total_elapsed = 0;

        //声明变量
        ThreadSafeQueue channel[NUMBER_OF_THREADS]; //信道
        thread_ctx ctx[NUMBER_OF_THREADS]; //线程上下文
        pthread_t threads[NUMBER_OF_THREADS]; //线程
        int ret; //线程返回值
        unsigned char* M = malloc(SPX_MLEN); //消息明文

        //生成安全素数
        prime = BN_new();
        init_crypto_params(prime);

#ifndef NOTPRINT
        printf("lagrange prime = %s\n", BN_bn2dec(prime));
#endif
    
        //初始化屏障
        pthread_barrier_init(&barrier, NULL, PLAYERS);

#ifndef NOTPRINT
        printf("Threshold Testing (n = %d, t = %d)\n", PLAYERS, THRESHOLD);
#endif

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
#ifndef NOTPRINT
        printf("KEYGEN: start...\n");
#endif
        timespec_get(&start_kg, TIME_UTC);

        pthread_create(&threads[0], NULL, keygen_TTP_logic , &ctx[0]);
        for (int i = 1;i < NUMBER_OF_THREADS;i++) {
            pthread_create(&threads[i], NULL, keygen_player_logic , &ctx[i]);
        }
        for (int i = 0;i < NUMBER_OF_THREADS;i++) {
            pthread_join(threads[i], NULL);
        }

        timespec_get(&end_kg, TIME_UTC);
#ifndef NOTPRINT
        printf("KEYGEN: successful\n");
#endif
        elapsed = (end_kg.tv_sec - start_kg.tv_sec) +
                  (end_kg.tv_nsec - start_kg.tv_nsec) / 1e9;
#ifndef NOTPRINT
        printf("\t time: %f s\n", elapsed);
#endif
        total_elapsed += elapsed;

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

#ifndef NOTPRINT
        printf("message:\t\t");
        for (int i = 0;i < SPX_MLEN;i++) {
            printf("%02x", M[i]);
        }
        printf("\n");
#endif

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
    
#ifndef NOTPRINT
        printf("Threshold players:\t");
        for (int i = 0;i < THRESHOLD;i++) {
            printf("%d ", threshold[i]);
        }
        printf("\n");
#endif

        //签名
#ifndef NOTPRINT
        printf("SIGN: start...\n");
#endif
        timespec_get(&start_sign, TIME_UTC);

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

        timespec_get(&end_sign, TIME_UTC);
#ifndef NOTPRINT
        printf("SIGN: successful\n");
#endif
        elapsed = (end_sign.tv_sec - start_sign.tv_sec) +
                (end_sign.tv_nsec - start_sign.tv_nsec) / 1e9;
#ifndef NOTPRINT
        printf("\t time: %f s\n", elapsed);
#endif
        total_elapsed += elapsed;

        unsigned char sigma[ctx[threshold[0]].smlen];
        memcpy(sigma, (&ctx[threshold[0]])->sm - ctx[threshold[0]].smlen,
            ctx[threshold[0]].smlen);
    
#ifndef NOTPRINT
        print_sigma(sigma);
#endif

        //验证
#ifndef NOTPRINT
        printf("VERIFY: start...\n");
#endif

        timespec_get(&start_vrfy, TIME_UTC);
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

        timespec_get(&end_vrfy, TIME_UTC);
#ifndef NOTPRINT
        printf("VERIFY: successful\n");
#endif
        elapsed = (end_vrfy.tv_sec - start_vrfy.tv_sec) +
                  (end_vrfy.tv_nsec - start_vrfy.tv_nsec) / 1e9;
        
#ifndef NOTPRINT
        printf("\t time: %f s\n", elapsed);
#endif
        total_elapsed += elapsed;

#ifndef NOTPRINT
        printf("Total time: %f s\n", total_elapsed);
#endif
        total_time += total_elapsed;
    }

#ifdef TIMETEST
    printf("SIGMA size: %.2f KB\n", SPX_BYTES / 1024.0);
    const char* filename = "benchmark_results.csv";
    write_to_csv(filename, params_env, thash_env, total_time / TURNS);
#endif

    return 0;
}