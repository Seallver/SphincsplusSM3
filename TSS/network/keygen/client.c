#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net_settings.h"

#include "keygen_round.h"
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
    pthread_barrier_init(&barrier, NULL, PLAYERS);
}

int main(int argc, char* argv[]) {
    init_params();
    
    if (argc != 2) {
        printf("Usage: %s <party_id>\n", argv[0]);
        return -1;
    }
    
    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx,PLAYERS);
    ctx->sss_ctx = sss_ctx;

    ctx->t = THRESHOLD;
    ctx->n = PLAYERS;

    for (int i = 0; i <= PLAYERS; i++) {
        memcpy(ctx->ip_[i], IP[i], strlen(IP[i]));
        ctx->port_[i] = port[i];
    }

    ctx->party_id = atoi(argv[1]);
    ctx->local_ip = malloc(strlen(IP[ctx->party_id]) + 1);
    memcpy(ctx->local_ip, IP[ctx->party_id], strlen(IP[ctx->party_id]));
    ctx->port = port[ctx->party_id];


    //执行keygen_round
    if (keygen_round_player(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }

    // 把keygen生成的关键数据输出(写入文件)
    char filename[256];
    snprintf(filename, sizeof(filename), "../data/party_%d_keygen_res.json", ctx->party_id);    
    if (save_ctx_to_file(ctx, filename)) {
        printf("Failed to save context\n");
    } else {
        printf("Results saved successfully\n");
    }

    return 0;
}