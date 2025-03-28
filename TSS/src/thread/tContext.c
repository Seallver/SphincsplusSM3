#include "tContext.h"

//初始化参与方的ctx
int init_ctx(thread_ctx* ctx, int tid,  ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list) {
    ctx->m = malloc(SPX_MLEN);

    ctx->sm = malloc(SPX_BYTES + SPX_MLEN);
    ctx->mout = malloc(SPX_BYTES + SPX_MLEN);
    ctx->tid = tid;
    ctx->mlen = SPX_MLEN;
    ctx->smlen = 0;

    memset(ctx->wots_addr, 0, sizeof(ctx->wots_addr));
    memset(ctx->tree_addr, 0, sizeof(ctx->tree_addr));

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

int input_m(thread_ctx* ctx, unsigned char* M ) {
    memcpy(ctx->m, M, SPX_MLEN);
    return 0;
}