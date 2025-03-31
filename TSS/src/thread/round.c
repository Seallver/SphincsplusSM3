#include "round.h"


//互斥锁，调试用输出函数
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void print_char(unsigned char* chr, int len, int tid) {
    pthread_mutex_lock(&mutex);
    printf("Thread %d: \t", tid);
    for (int i = 0; i < len; i++) {
        printf("%x", chr[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
}



void keygen_round_ttp(thread_ctx* thread_ctx) {
    //TTP无记忆
    thread_ctx->sss_ctx = NULL;

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
    //声明并初始化SSS参数
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx);
    thread_ctx->sss_ctx = sss_ctx;
    
    BN_CTX* BNctx = BN_CTX_new();
    //生成共享份额
    BIGNUM** tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 1; i < PLAYERS + 1; i++) {
        tmp_shares[i] = BN_new();
    }
    generate_shares(tmp_shares, sss_ctx->coeffs, BNctx);

    //p2p传递共享份额
    keygen_player_p2p_shares(thread_ctx, tmp_shares);

    //接收共享份额并聚合
    keygen_player_recv_shares(thread_ctx, BNctx);

    //等待共享份额计算完毕
    pthread_barrier_wait(&barrier);

    //任意两方协商随机数
    keygen_randome_agreement(thread_ctx, BNctx);

    //向TTP发送秘密份额
    keygen_player_p2ttp_shards(thread_ctx, thread_ctx->sss_ctx->secret, BNctx);

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
    BN_copy(shares, ctx->sss_ctx->share);
    generate_threshold_shards(ctx->sss_ctx->share, shares, ctx->tid);

    //向第三方发送共享秘密份额
    presign_player_p2ttp_shards(ctx);

    //接收R
    presign_player_recv_R(ctx);

    //接收FORS私钥种子
    presign_player_recv_seed(ctx);

    //生成地址编码并签名FORS
    // tss_sign_FORS(ctx);
    tss_sign_FORS(ctx->sk, ctx->pk, ctx->wots_addr, ctx->mhash, ctx->root, ctx->m, ctx->sm, &ctx->smlen, ctx->mlen, &ctx->tree, &ctx->idx_leaf);
    ctx->sm += SPX_N + SPX_FORS_BYTES;

    BN_free(shares);

}

void sign_round_player(thread_ctx* ctx) {
    //计算出自己WOTS签名的地址
    tss_gen_addr(ctx->tid, &ctx->tree, &ctx->idx_leaf, ctx->wots_addr, ctx->tree_addr);

    //初始化签名准备
    unsigned char* sk = malloc(SPX_SK_BYTES);
    int len = BN_bn2bin(ctx->sss_ctx->secret, sk);
    memcpy(ctx->sk, sk, SPX_N);
    unsigned char* sig_shards = malloc(SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);

    
    bool flag = false;
    //判断是否是第一层WOTS签名方
    if (ctx->tid == threshold[0]) {
        //生成签名
        tss_sign_WOTS(ctx->sk, ctx->pk, sig_shards, ctx->root, ctx->wots_addr, ctx->tree_addr, ctx->idx_leaf);
        memcpy(ctx->sm, sig_shards, SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
        ctx->sm += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
        ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
        //广播签名份额
        sign_bc_sig_shards(ctx, sig_shards);
        //向下一个门限层发送root
        sign_p2p_root(ctx, threshold[1]);

        flag = true;
    }

    
    //接收其他方发来的签名份额
    for (int i = 1;i <= THRESHOLD;i++) {
        //若上一轮由自己签名，跳过本次接收
        if (flag) {
            flag = false;
            continue;
        }
        //接收其他参与方的签名份额并聚合
        sign_recv_sig_shards(ctx, sig_shards);
        memcpy(ctx->sm, sig_shards, SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
        ctx->sm += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
        ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
        if (threshold[i] == ctx->tid) {
            //接收root
            sign_recv_root(ctx);
            //签名，生成自己的签名份额并聚合
            tss_sign_WOTS(ctx->sk, ctx->pk, sig_shards, ctx->root, ctx->wots_addr, ctx->tree_addr, ctx->idx_leaf);
            memcpy(ctx->sm, sig_shards, SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
            ctx->sm += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
            ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
            if (i == THRESHOLD - 1) {
                //向TTP发送root
                sign_p2p_root(ctx, 0);
                //广播消息
                sign_bc_sig_shards(ctx, sig_shards);
                flag = true;
            }
            else {
                //广播消息
                sign_bc_sig_shards(ctx, sig_shards);
                //向下一个门限层发送root
                sign_p2p_root(ctx, threshold[i + 1]);
                //标记下轮接收消息可跳过
                flag = true;
            }
        }
    }

    //等待接收最终签名份额
    sign_recv_sig_shards(ctx, sig_shards);
    memcpy(ctx->sm, sig_shards, SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
    ctx->sm += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;

    
}


void sign_round_ttp(thread_ctx* ctx) {
    //接收并聚合共享秘密份额得到Seed
    BIGNUM* Seed = BN_new();
    ttp_recv_lagrange_shards(ctx, Seed);

    //生成私钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(Seed));
    int len = BN_bn2bin(Seed, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    tss_crypto_sign_keypair(pk, sk, seed);

    memcpy(ctx->pk, pk, SPX_PK_BYTES);

    //生成随机数R
    unsigned char* R = malloc(SPX_BYTES);
    tss_gen_R(R, ctx->m, ctx->mlen, sk);
    
    //广播R
    presign_ttp_bc_R(ctx, R);

    //生成FORS的私钥种子
    unsigned char fors_seed[SPX_N];
    tss_gen_FORS_seed(fors_seed);

    //广播FORS的私钥种子
    presign_ttp_bc_FORS_seed(ctx, fors_seed);

    //计算对顶层签名所需要的地址
    tss_gen_ttp_addr(ctx->pk, ctx->mhash, ctx->m, ctx->mlen,
        &ctx->tree, &ctx->idx_leaf, ctx->wots_addr, ctx->tree_addr, R);

    //接收顶层root为签名做准备
    sign_recv_root(ctx);

    //生成顶层签名份额
    unsigned char* sig_shards = malloc(SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
    tss_sign_WOTS_ttp(ctx->pk, ctx->root, ctx->wots_addr,
        ctx->tree_addr, ctx->idx_leaf, sig_shards, sk);

    //广播顶层签名份额
    sign_bc_sig_shards(ctx, sig_shards);

}


int vrfy_round_player(thread_ctx* ctx) {
    //还原sm指针
    ctx->sm -= ctx->smlen;

    //把消息级联到签名后面方便验证
    memmove(ctx->sm + SPX_BYTES, ctx->m, ctx->mlen);

    //验证签名
    if (tss_crypto_sign_verify(ctx->sm, SPX_BYTES, ctx->sm + SPX_BYTES, ctx->mlen, ctx->pk)) {
        return -1;
    }
    return 0;
}