#include "keygen_round.h"


int keygen_round_player(KeygenNet_ctx* ctx) {
    BN_CTX* BNctx = BN_CTX_new();
    //生成共享份额
    ctx->tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 1; i <= PLAYERS; i++) {
        ctx->tmp_shares[i] = BN_new();
    }
    generate_shares(ctx->tmp_shares, ctx->sss_ctx->coeffs, BNctx);
    
    //监听本机端口，等待连接，P2P交换共享份额
    int conn_numbers = ctx->party_id - 1;
    if (conn_numbers)
        keygen_listen_local_port(ctx, conn_numbers, player_handler_recv);

    //主动与其他参与方建立连接
    for (int i = ctx->party_id + 1; i <= PLAYERS; i++) {
        keygen_create_connection_p2p(ctx->ip_[i], ctx->port_[i], ctx, player_handler_send);
    }

    //聚合共享份额
    aggregate_shares(ctx->sss_ctx->share, ctx->tmp_shares + 1, BNctx);

    //发送盲化后的份额给ttp，并接收公钥
    keygen_create_connection_p2p(ctx->ip_[0], ctx->port_[0], ctx, player_handler_send_ttp);

    BN_CTX_free(BNctx);
    return 0;
}

int keygen_round_ttp(KeygenNet_ctx* ctx) {
    ctx->tmp_shares = (BIGNUM**)malloc((PLAYERS + 1) * sizeof(BIGNUM*));
    for(int i = 1; i <= PLAYERS; i++) {
        ctx->tmp_shares[i] = BN_new();
    }

    //起一个线程等待计算总公私钥
    pthread_t thread;
    pthread_create(&thread, NULL, complete_pk, ctx);

    //监听本机端口，等待连接，此次连接需要收集盲化后的私钥分片并聚合，然后生成公钥再发回去
    int conn_numbers = PLAYERS;
    keygen_listen_local_port(ctx, conn_numbers, ttp_handler_recv);

    return 0;
}