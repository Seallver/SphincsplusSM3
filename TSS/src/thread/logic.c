#include "logic.h"
#include "globals.h"

//kg参与方线程逻辑
int keygen_player_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;

    printf("Keygen: Thread %u start\n", tid);
    /* Make stdout buffer more responsive. */
    setbuf(stdout, NULL);
    
    unsigned char *pk = &(ctx->pk);
    unsigned char *sk = &(ctx->sk);
    unsigned char *m = ctx->m;
    unsigned char *sm = ctx->sm;
    unsigned char *mout = ctx->mout;
    unsigned long long smlen;
    unsigned long long mlen;

    keygen_round_player(ctx);

    //到达屏障阻塞，等待其他线程接收完毕
    pthread_barrier_wait(&barrier);

    presign_round(ctx);
    
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

int presign_logic(thread_ctx* ctx) {
    unsigned int tid = ctx->tid;
    printf("Sign: Thread %u start\n", tid);

    presign_round(ctx);

    printf("Sign: Thread %u end\n", tid);
    return 0;
}