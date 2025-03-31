#include "tContext.h"

//初始化参与方的ctx
int init_ctx(thread_ctx* ctx, int tid, unsigned long long mlen ,ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list) {
    ctx->m = malloc(mlen);

    ctx->sm = malloc(SPX_BYTES + mlen);
    ctx->mout = malloc(SPX_BYTES + mlen);
    ctx->tid = tid;
    ctx->mlen = mlen;
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
    memcpy(ctx->m, M, ctx->mlen);
    return 0;
}