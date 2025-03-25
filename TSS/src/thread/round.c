#include "round.h"
#include "globals.h"

//互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void keygen_round_ttp(thread_ctx* thread_ctx) {
    //声明并初始化VSS参数
    TTP_VSS_ctx* vss_ctx = (TTP_VSS_ctx*)malloc(sizeof(TTP_VSS_ctx));
    TTP_VSS_init(vss_ctx, thread_ctx->p, thread_ctx->g);
    thread_ctx->ttp_vss_ctx = vss_ctx;
    thread_ctx->vss_ctx = NULL;
    
    //p2p发送shares
    keygen_p2p_shares(thread_ctx);

    //广播comms
    keygen_bc_comms(thread_ctx);

    //生成公钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(vss_ctx->secret));
    int len = BN_bn2bin(vss_ctx->secret, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    tss_crypto_sign_keypair(thread_ctx->pk, thread_ctx->sk, seed);

    //广播公钥
    keygen_bc_pk(thread_ctx);


}

void keygen_round_player(thread_ctx* thread_ctx) {
    //声明并初始化VSS参数
    VSS_ctx* vss_ctx = (VSS_ctx*)malloc(sizeof(VSS_ctx));
    VSS_init(vss_ctx, thread_ctx->p, thread_ctx->g);
    thread_ctx->vss_ctx = vss_ctx;
    thread_ctx->ttp_vss_ctx = NULL;
    

    int* pid = (int*)malloc(sizeof(int));

    //从TTP接收shares
    keygen_recv_shares(thread_ctx);


    //从TTP接收comms
    keygen_recv_comms(thread_ctx);

    //接收公钥
    keygen_recv_pk(thread_ctx);

    // //验证承诺
    // BIGNUM *x = BN_new();
    // BN_set_word(x, (unsigned long)(thread_ctx->tid));
    // int ret = verify_share(vss_ctx->share, x, vss_ctx->comms, THRESHOLD - 1, vss_ctx->p, vss_ctx->g);
    // if (ret == 0) {
    //     printf("verify_share failed\n");
    //     exit(1);
    // }
    // else {
    //     if (IS_PRINT)
    //         printf("verify_share success\n");
    // }

    free(pid);
}


void presign_round(thread_ctx* ctx) {
    //分享自己的秘密份额
    sign_bc_shares(ctx);

    BIGNUM** tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 0; i <= PLAYERS; i++) {
        tmp_shares[i] = BN_new();
        BN_zero(tmp_shares[i]);
    }
    BN_copy(tmp_shares[ctx->tid], ctx->vss_ctx->share);
    for (int i = 0; i < THRESHOLD - 1; i++) {
        //接收其他参与方的秘密份额
        sign_recv_shares(ctx, tmp_shares);
    }

    // //输出各接收方收到的秘密份额
    // pthread_mutex_lock(&mutex);
    // printf("tid: %d\n", ctx->tid);
    // for (int i = 0; i <= PLAYERS; i++) {
    //     printf("sign_shares[%d] = %s\n", i, BN_bn2dec(tmp_shares[i]));
    // }
    // pthread_mutex_unlock(&mutex);



    for (int i = 0; i <= PLAYERS; i++) {
        BN_free(tmp_shares[i]);
    }


}
