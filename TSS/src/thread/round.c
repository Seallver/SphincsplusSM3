#include "round.h"
#include "globals.h"

//互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void keygen_round_ttp(thread_ctx* thread_ctx) {
    //TTP无记忆
    thread_ctx->vss_ctx = NULL;

    //接收秘密份额并聚合为Seed
    BIGNUM* Seed = BN_new();
    keygen_ttp_recv_shards(thread_ctx, Seed);
    printf("Seed: %s\n",BN_bn2dec(Seed));
    //生成公钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(Seed));
    int len = BN_bn2bin(Seed, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    tss_crypto_sign_keypair(pk, sk, seed);

    //广播公钥
    keygen_ttp_bc_pk(thread_ctx, pk);

    free(seed);
    BN_free(Seed);
}

void keygen_round_player(thread_ctx* thread_ctx) {
    //声明并初始化VSS参数
    VSS_ctx* vss_ctx = (VSS_ctx*)malloc(sizeof(VSS_ctx));
    VSS_init(vss_ctx);
    thread_ctx->vss_ctx = vss_ctx;

    BN_CTX *BNctx = BN_CTX_new();
    //生成共享份额
    BIGNUM** tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 1; i < PLAYERS + 1; i++) {
        tmp_shares[i] = BN_new();
    }
    generate_shares(tmp_shares, vss_ctx->coeffs, BNctx);

    //p2p传递共享份额
    keygen_player_p2p_shares(thread_ctx, tmp_shares);

    //接收共享份额并聚合
    keygen_player_recv_shares(thread_ctx);

    //向TTP发送秘密份额
    keygen_player_p2ttp_shards(thread_ctx, thread_ctx->vss_ctx->secret);

    //接收公钥
    keygen_recv_pk(thread_ctx);

    BN_CTX_free(BNctx);
    for(int i = 1; i <= PLAYERS; i++) {
        BN_free(tmp_shares[i]);
    }
    free(tmp_shares);

}


void presign_round_player(thread_ctx* ctx) {
    //计算门限份额
    BIGNUM* shares = BN_new();
    BN_copy(shares, ctx->vss_ctx->share);
    generate_threshold_shards(ctx->vss_ctx->share, shares, ctx->tid);

    //向第三方发送共享秘密份额
    presign_player_p2ttp_shards(ctx);

    //接收SPX路径和消息摘要

}


void presign_round_ttp(thread_ctx* ctx) {
    //接收并聚合共享秘密份额得到Seed
    BIGNUM* Seed = BN_new();
    presign_ttp_recv_shards(ctx, Seed);

    //输出seed
    printf("Seed: %s\n",BN_bn2dec(Seed));

    //生成私钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(Seed));
    int len = BN_bn2bin(Seed, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    tss_crypto_sign_keypair(pk, sk, seed);

    //生成SPX签名路径和消息摘要

    //广播SPX路径和消息摘要
}