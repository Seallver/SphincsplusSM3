#include "sign_round.h"

int sign_round_player(SignNet_ctx* ctx) {
    BN_CTX* BNctx = BN_CTX_new();

    //计算门限份额
    BIGNUM* shares = BN_new();
    BN_copy(shares, ctx->share);
    generate_threshold_shards(ctx->share, shares, ctx->party_id);

    //发送共享份额给ttp，并等待接收R和FORS私钥种子
    sign_create_connection_p2p(IP[0], port[0], ctx, player_conn_to_ttp);

    //初始化结构体参数进行FORS签名
    ctx->sm = malloc(SPX_BYTES + ctx->mlen);
    ctx->smlen = 0;

    memset(ctx->wots_addr, 0, sizeof(ctx->wots_addr));
    memset(ctx->tree_addr, 0, sizeof(ctx->tree_addr));

    tss_sign_FORS(ctx->sk, ctx->pk, ctx->wots_addr, ctx->mhash, ctx->root, ctx->m, ctx->sm, &ctx->smlen, ctx->mlen, &ctx->tree, &ctx->idx_leaf);
    ctx->sm += SPX_N + SPX_FORS_BYTES;

    //计算出自己WOTS签名的地址
    tss_gen_addr(ctx->tid, &ctx->tree, &ctx->idx_leaf, ctx->wots_addr, ctx->tree_addr);


    BN_free(shares);
    BN_CTX_free(BNctx);
    return 0;
}

int sign_round_ttp(SignNet_ctx* ctx) {
    ctx->tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 1; i <= PLAYERS; i++) {
        ctx->tmp_shares[i] = BN_new();
        BN_zero(ctx->tmp_shares[i]);
    }

    //起一个线程等待计算总公私钥
    pthread_t thread;
    pthread_create(&thread, NULL, aggregate_seed , ctx);

    //监听本机端口，等待连接，此次连接需要收集共享份额并聚合，然后生成R和FORS私钥种子并发送回去
    int conn_numbers = THRESHOLD;
    sign_listen_local_port(ctx, conn_numbers, ttp_listen_player);


    return 0;

}