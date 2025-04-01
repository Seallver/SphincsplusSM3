#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sign_round.h"
#include "Json.h"

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
    pthread_barrier_init(&barrier, NULL, THRESHOLD);
}

int main(int argc, char* argv[]) {
    init_params();

    if (argc != 2) {
        printf("Usage: %s <party_id>\n", argv[0]);
        return -1;
    }

    // 从文件加载
    SignNet_ctx* ctx = (SignNet_ctx*)malloc(sizeof(SignNet_ctx));
    ctx->party_id = atoi(argv[1]);


    const char filename[256];
    snprintf(filename, sizeof(filename), "../data/party_%d_keygen_res.json", ctx->party_id);
    int ret = load_ctx_from_file(ctx, filename);
    if (ret) {
        printf("Failed to load context\n");
        return 1;
    }
    ctx->mlen = MLEN;
    ctx->m = malloc(ctx->mlen);
    memcpy(ctx->m, M, ctx->mlen);

    printf("party_id: %d\n", ctx->party_id);

    printf("IP: %s\n", ctx->local_ip);
    //执行sign_round
    if (sign_round_player(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }



    return 0;
}