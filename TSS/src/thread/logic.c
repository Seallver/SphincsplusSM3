#include "logic.h"


int keygen_player_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;
    // if (IS_PRINT)
    //     printf("Keygen: Thread %u start\n", tid);

    keygen_round_player(ctx);

    //到达屏障阻塞，等待其他线程接收完毕
    pthread_barrier_wait(&barrier);

    // if (IS_PRINT)
    //     printf("Keygen: Thread %u end\n", tid);
    return 0;
}


int keygen_TTP_logic(thread_ctx* ctx) {
    // if (IS_PRINT)
    //     printf("Keygen: Thread 0 start (TTP)\n");

    keygen_round_ttp(ctx);

    // if (IS_PRINT)
    //     printf("Keygen: Thread 0 end (TTP)\n");
    return 0;
}

int sign_player_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;
    // if (IS_PRINT)
    //     printf("Sign: Thread %u start\n", tid);

    presign_round_player(ctx);

    //到达屏障阻塞，等待其他线程预签名完成
    pthread_barrier_wait(&barrier);

    sign_round_player(ctx);

    // if (IS_PRINT)
    //     printf("Sign: Thread %u end\n", tid);
    return 0;
}

int sign_TTP_logic(thread_ctx* ctx) {
    // if (IS_PRINT)
    //     printf("Sign: Thread 0 start (TTP)\n");

    sign_round_ttp(ctx);

    // if (IS_PRINT)
    //     printf("Sign: Thread 0 end (TTP)\n");
    return 0;
}

int vrfy_player_logic(thread_ctx* ctx) {
    // if (IS_PRINT)
    //     printf("Verify: Thread %u start\n", ctx->tid);

    int ret = vrfy_round_player(ctx);
    
    // if (IS_PRINT)
    //     printf("Verify: Thread %u end\n", ctx->tid);

    return ret;
}

