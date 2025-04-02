#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keygen_connection.h"


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
    pthread_barrier_init(&barrier, NULL, PLAYERS + 1);

}

int main() {
    init_params();

    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx);
    ctx->sss_ctx = sss_ctx;

    ctx->party_id = 0;
    ctx->local_ip = malloc(strlen(IP[0]) + 1);
    memcpy(ctx->local_ip, IP[0], strlen(IP[0]));
    ctx->port = port[0];

    //执行keygen_round
    if (keygen_round_ttp(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }

    return 0;
}