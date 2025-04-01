#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keygen_round.h"


BIGNUM* prime;
int threshold[PLAYERS];
pthread_barrier_t barrier;

void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, PLAYERS);
}

int main(int argc, char* argv[]) {
    init_params();
    
    if (argc != 4) {
        printf("Usage: %s <party_id> <local_IP> <port> \n", argv[0]);
        return -1;
    }

    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx);
    ctx->sss_ctx = sss_ctx;

    ctx->party_id = atoi(argv[1]);
    ctx->local_ip = malloc(strlen(argv[2]) + 1);
    memcpy(ctx->local_ip, argv[2], strlen(argv[2]));
    ctx->port = atoi(argv[3]);

    //执行keygen_round
    if (keygen_round_player(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }
    
    return 0;
}