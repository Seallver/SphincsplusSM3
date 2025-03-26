#include "round.h"
#include "globals.h"

//互斥锁，输出函数
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void print_char(unsigned char* chr, int len) {
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < len ; i++) {
        printf("%x", chr[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
}



void keygen_round_ttp(thread_ctx* thread_ctx) {
    //TTP无记忆
    thread_ctx->vss_ctx = NULL;

    //接收秘密份额并聚合为Seed
    BIGNUM* Seed = BN_new();
    keygen_ttp_recv_shards(thread_ctx, Seed);

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
    
    BN_CTX* BNctx = BN_CTX_new();
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
    //初始化变量
    unsigned long long smlen;
    unsigned char* seed = malloc(SPX_N);
    //计算门限份额
    BIGNUM* shares = BN_new();
    BN_copy(shares, ctx->vss_ctx->share);
    generate_threshold_shards(ctx->vss_ctx->share, shares, ctx->tid);

    //向第三方发送共享秘密份额
    presign_player_p2ttp_shards(ctx);

    //接收R
    presign_player_recv_R(ctx);

    //接收FORS私钥种子
    presign_player_recv_seed(ctx, seed);

    //生成地址编码并签名FORS
    tss_sign_FORS(ctx->sm, ctx->m, ctx->mlen, seed, ctx->pk, ctx->R);

    free(seed);
    BN_free(shares);

}


void presign_round_ttp(thread_ctx* ctx) {
    //接收并聚合共享秘密份额得到Seed
    BIGNUM* Seed = BN_new();
    presign_ttp_recv_shards(ctx, Seed);

    //生成私钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(Seed));
    int len = BN_bn2bin(Seed, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    tss_crypto_sign_keypair(pk, sk, seed);

    //生成随机数R
    unsigned char* R = malloc(SPX_BYTES);
    tss_gen_R(R, ctx->m, sk);
    
    //广播R
    presign_ttp_bc_R(ctx, R);

    //生成FORS的私钥种子
    unsigned char fors_seed[SPX_N];
    tss_gen_FORS_seed(fors_seed);

    //广播FORS的私钥种子
    presign_ttp_bc_FORS_seed(ctx, fors_seed);

}

void sign_round(thread_ctx* ctx) {
    
}