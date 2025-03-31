#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net_settings.h"
#include "keygen_connection.h"
#include "net_context.h"

BIGNUM* prime;
int threshold[PLAYERS];

void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }
}

int main(int argc, char* argv[]) {
    init_params();
    
    if (argc != 4) {
        printf("Usage: %s <party_id> <local_IP> <port> \n", argv[0]);
        return -1;
    }

    //初始化参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx);
    ctx->sss_ctx = sss_ctx;

    ctx->party_id = atoi(argv[1]);
    ctx->local_ip = malloc(strlen(argv[2]) + 1);
    memcpy(ctx->local_ip, argv[2], strlen(argv[2]));
    ctx->port = atoi(argv[3]);

    //监听本机端口，等待连接
    int conn_numbers = ctx->party_id - 1;
    listen_local_port(ctx, conn_numbers, player_handler_recv);

    //主动与其他参与方建立连接
    for (int i = ctx->party_id + 1; i <= PLAYERS; i++) {
        create_connection_p2p(IP[i], port[i], ctx, player_handler_send);
    }
    
    return 0;
}