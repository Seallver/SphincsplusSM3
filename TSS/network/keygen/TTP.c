#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net_settings.h"
#include "keygen_connection.h"

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

    if (argc != 3) {
        printf("Usage: %s <local_IP> <port> \n", argv[0]);
        return -1;
    }

    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    ctx->party_id = 0;
    ctx->local_ip = malloc(strlen(argv[1]) + 1);
    memcpy(ctx->local_ip, argv[1], strlen(argv[1]));
    ctx->port = atoi(argv[2]);

    //监听本机端口，等待连接
    int conn_numbers = PLAYERS;
    listen_local_port(ctx, conn_numbers, ttp_handler_recv);

    //主动与其他参与方建立连接
    for (int i = 1; i <= PLAYERS; i++) {
        create_connection_p2p(IP[i], port[i], ctx, ttp_handler_send);
    }



    return 0;
}