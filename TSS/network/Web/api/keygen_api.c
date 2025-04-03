#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "net_settings.h"

#include "keygen_round.h"
#include "Json.h"

BIGNUM* prime;
int threshold[SPX_D - 1];
pthread_barrier_t barrier;

//所需参数形式：<n> <t> <party_id> + <threshold_id> <threshold_id> ... + <IP> <port> <IP> <port> ...
KeygenNet_ctx* playerAPI(int n, int t, int party_id,...) {
    if (n > SPX_D - 1 || n < 0) {
        printf("n must be in [0, d)\n");
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("t must be in (0, n)\n");
        return NULL;
    }
        
    va_list args;
    va_start(args, party_id);

    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx, n);
    ctx->sss_ctx = sss_ctx;
    ctx->party_id = party_id;
    
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < t; i++) {
        threshold[i] = va_arg(args, int);
    }

    ctx->local_ip = malloc(strlen(ctx->ip_[ctx->party_id]) + 1);
    memcpy(ctx->local_ip, ctx->ip_[ctx->party_id], strlen(ctx->ip_[ctx->party_id]));
    ctx->port = ctx->port_[ctx->party_id];

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, n);   

    for (int i = 0; i <= n; i++) {
        char* ipaddr = va_arg(args, char*);
        memcpy(ctx->ip_[i], ipaddr, strlen(ipaddr));
        ctx->port_[i] = va_arg(args, int);
    }

    //执行keygen_round
    if (keygen_round_player(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }

    // 把keygen生成的关键数据输出(写入文件)
    char filename[256];
    snprintf(filename, sizeof(filename), "party_%d_keygen_res.json", ctx->party_id);    
    if (save_ctx_to_file(ctx, filename)) {
        printf("Failed to save context\n");
    } else {
        printf("Results saved successfully\n");
    }

    return ctx;
}

//所需参数形式:<n> <t> <threshold_id> <threshold_id> ... + <IP> <port> <IP> <port> ...
KeygenNet_ctx* ttpAPI(int n, int t, ...) {
    if (n > SPX_D - 1 || n < 0) {
        printf("n must be in [0, d)\n");
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("t must be in (0, n)\n");
        return NULL;
    }
        
    va_list args;
    va_start(args, t);

    
    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));
    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx, n);
    ctx->sss_ctx = sss_ctx;

    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < t; i++) {
        threshold[i] = va_arg(args, int);
    }

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, n);   

    for (int i = 0; i <= n; i++) {
        char* ipaddr = va_arg(args, char*);
        memcpy(ctx->ip_[i], ipaddr, strlen(ipaddr));
        ctx->port_[i] = va_arg(args, int);
    }

    ctx->party_id = 0;
    ctx->local_ip = malloc(strlen(ctx->ip_[0]) + 1);
    memcpy(ctx->local_ip, ctx->ip_[0], strlen(ctx->ip_[0]));
    ctx->port = ctx->port_[0];

    //执行keygen_round
    if (keygen_round_ttp(ctx)) {
        printf("%d: keygen failed\n", ctx->party_id);   
    }

    return ctx;
}