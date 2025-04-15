#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net_settings.h"
#include "params.h"
#include "sign_round.h"
#include "Json.h"

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

    ctx->t = THRESHOLD;
    ctx->n = PLAYERS;

    for (int i = 0; i <= PLAYERS; i++) {
        memcpy(ctx->ip_[i], IP[i], strlen(IP[i]));
        ctx->port_[i] = port[i];
    }

    //执行sign_round
    if (sign_round_player(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }

    ctx->sm -= ctx->smlen;
    printf("sm size: %u\n", ctx->smlen);
    printf("size: %u\n", SPX_BYTES);
    printf("message size: %u\n", (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N));

    memmove(ctx->sm + SPX_BYTES, ctx->m, ctx->mlen);

    // 把keygen生成的关键数据输出(写入文件)
    snprintf(filename, sizeof(filename), "../data/party_%d_sig.json", ctx->party_id);    
    if (save_sig_to_file(ctx, filename, SPX_BYTES, threshold)) {
        printf("Failed to save context\n");
    } else {
        printf("Results saved successfully\n");
    }
    
    if (tss_crypto_sign_verify(ctx->sm, SPX_BYTES, ctx->sm + SPX_BYTES, ctx->mlen, ctx->pk)) {
        printf("vrfy failed\n");
        return -1;
    }
    printf("vrfy success!\n");

    return 0;
}