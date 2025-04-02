#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sign_connection.h"


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
    pthread_barrier_init(&barrier, NULL, THRESHOLD + 1);

}

int main(int argc, char* argv[]) {
    init_params();

    if (argc != 3) {
        printf("Usage: %s <local_IP> <port> \n", argv[0]);
        return -1;
    }

    //初始化上下文参数
    SignNet_ctx* ctx = (SignNet_ctx*)malloc(sizeof(SignNet_ctx));

    ctx->party_id = 0;
    ctx->local_ip = malloc(strlen(argv[1]) + 1);
    memcpy(ctx->local_ip, argv[1], strlen(argv[1]));
    ctx->port = atoi(argv[2]);

    ctx->mlen = MLEN;
    ctx->m = malloc(ctx->mlen);
    memcpy(ctx->m, M, ctx->mlen);

    //执行sign_round
    if (sign_round_ttp(ctx)) {
        printf("%d: sign failed\n", ctx->party_id);   
    }

    return 0;
}