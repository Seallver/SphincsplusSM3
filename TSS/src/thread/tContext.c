#include "tContext.h"

//初始化参与方的ctx
int init_ctx(thread_ctx* ctx, int tid, unsigned char* M, ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list, BIGNUM* p, BIGNUM* g) {
    ctx->m = malloc(SPX_MLEN);
    memcpy(ctx->m, M, SPX_MLEN);

    ctx->sm = malloc(SPX_BYTES + SPX_MLEN);
    ctx->mout = malloc(SPX_BYTES + SPX_MLEN);
    ctx->tid = tid;

    ctx->self_channel = channel;
    ctx->public_channel_list = public_channel_list;

    ctx->p = p;
    ctx->g = g;

    if (ctx->m == NULL || ctx->sm == NULL || ctx->mout == NULL) {
        free(ctx->m);
        free(ctx->sm);
        free(ctx->mout);
        return -1; // 初始化失败
    }

    return 0; // 初始化成功
}