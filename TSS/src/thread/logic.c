#include "logic.h"


//kg参与方线程逻辑
int keygen_player_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;

    printf("Keygen: Thread %u start\n", tid);

    keygen_round_player(ctx);

    //到达屏障阻塞，等待其他线程接收完毕
    pthread_barrier_wait(&barrier);
    
    printf("Keygen: Thread %u end\n", tid);
    return 0;
}

//kg可信第三方线程逻辑
int keygen_TTP_logic(thread_ctx* ctx) {
    printf("Keygen: Thread 0 start (TTP)\n");

    keygen_round_ttp(ctx);

    printf("Keygen: Thread 0 end (TTP)\n");
    return 0;
}

int sign_player_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;
    printf("Sign: Thread %u start\n", tid);

    presign_round_player(ctx);

    pthread_barrier_wait(&barrier);

    sign_round_player(ctx);

    printf("Sign: Thread %u end\n", tid);
    return 0;
}

int sign_TTP_logic(thread_ctx* ctx) {
    printf("Sign: Thread 0 start (TTP)\n");

    presign_round_ttp(ctx);

    sign_round_ttp(ctx);

    printf("Sign: Thread 0 end (TTP)\n");
    return 0;
}