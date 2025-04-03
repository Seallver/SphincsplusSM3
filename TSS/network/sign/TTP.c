#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sign_connection.h"
#include "net_settings.h"

BIGNUM* prime;
int threshold[SPX_D - 1];
pthread_barrier_t barrier;

void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, THRESHOLD + 1);

}

int main() {
    init_params();

    //初始化上下文参数
    SignNet_ctx* ctx = (SignNet_ctx*)malloc(sizeof(SignNet_ctx));

    for (int i = 0; i <= PLAYERS; i++) {
        memcpy(ctx->ip_[i], IP[i], strlen(IP[i]));
        ctx->port_[i] = port[i];
    }

    ctx->t = THRESHOLD;
    ctx->n = PLAYERS;

    ctx->party_id = 0;
    ctx->local_ip = malloc(strlen(IP[0]) + 1);
    memcpy(ctx->local_ip, IP[0], strlen(IP[0]));
    ctx->port = port[0];

    ctx->mlen = MLEN;
    ctx->m = malloc(ctx->mlen);
    memcpy(ctx->m, M, ctx->mlen);

    //执行sign_round
    if (sign_round_ttp(ctx)) {
        printf("%d: sign failed\n", ctx->party_id);   
    }

    return 0;
}