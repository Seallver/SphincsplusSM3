#include "round.h"

void keygen_round_ttp(thread_ctx* thread_ctx, BIGNUM* prime, BIGNUM* generator) {

    //声明并初始化VSS参数
    TTP_VSS_ctx* vss_ctx = (TTP_VSS_ctx*)malloc(sizeof(TTP_VSS_ctx));
    TTP_VSS_init(vss_ctx, prime, generator);

    thread_ctx->vss_ctx = vss_ctx;

    //p2p发送shares
    for (int i = 1; i <= PLAYERS; i++) {
        void* sptr = (void*)(vss_ctx->shares[i]);
        Send_Msg(thread_ctx->public_channel_list, 0, i, sptr);
    }

    //广播comms
    void* cptr = (void*)(vss_ctx->comms);
    Send_Msg(thread_ctx->public_channel_list, 0, -1, cptr);
}

void keygen_round_player(thread_ctx* thread_ctx, BIGNUM* prime, BIGNUM* generator) {
    //声明并初始化VSS参数
    VSS_ctx* vss_ctx = (VSS_ctx*)malloc(sizeof(VSS_ctx));
    VSS_init(vss_ctx, prime, generator);

    int* pid = (int*)malloc(sizeof(int));
    //从TTP接收shares
    void* smsg ;
    Recv_Msg(thread_ctx->self_channel, pid, &smsg);
    BN_copy(vss_ctx->share, (BIGNUM*)smsg);

    //从TTP接收comms
    void* recv;
    Recv_Msg(thread_ctx->self_channel, pid, &recv);

    BIGNUM** recvmsg = (BIGNUM**)recv;
    for (int i = 0; i < THRESHOLD; i++) {
        BN_copy(vss_ctx->comms[i], recvmsg[i]);
    }

    free(pid);

}