#include "round.h"

void keygen_round_ttp(thread_ctx* thread_ctx) {
    //声明并初始化VSS参数
    TTP_VSS_ctx* vss_ctx = (TTP_VSS_ctx*)malloc(sizeof(TTP_VSS_ctx));
    TTP_VSS_init(vss_ctx, thread_ctx->p, thread_ctx->g);
    thread_ctx->ttp_vss_ctx = vss_ctx;
    thread_ctx->vss_ctx = NULL;
    
    //p2p发送shares
    p2p_shares(vss_ctx, thread_ctx->public_channel_list);

    //广播comms
    bc_comms(vss_ctx, thread_ctx->public_channel_list);

    //生成公钥
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(vss_ctx->secret));
    int len = BN_bn2bin(vss_ctx->secret, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    tss_crypto_sign_keypair(thread_ctx->pk, thread_ctx->sk, seed);

    //广播公钥
    bc_pk(thread_ctx);


}

void keygen_round_player(thread_ctx* thread_ctx) {
    //声明并初始化VSS参数
    VSS_ctx* vss_ctx = (VSS_ctx*)malloc(sizeof(VSS_ctx));
    VSS_init(vss_ctx, thread_ctx->p, thread_ctx->g);
    thread_ctx->vss_ctx = vss_ctx;
    thread_ctx->ttp_vss_ctx = NULL;
    

    int* pid = (int*)malloc(sizeof(int));

    //从TTP接收shares
    recv_shares(thread_ctx);


    //从TTP接收comms
    recv_comms(thread_ctx);

    //接收公钥
    recv_pk(thread_ctx);

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
    
}
