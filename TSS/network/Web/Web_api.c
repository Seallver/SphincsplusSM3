// 确保编译器可见性设置
#if defined(__GNUC__) || defined(__clang__)
#define API_EXPORT __attribute__((visibility("default")))
#else
#define API_EXPORT
#endif

// 函数声明
API_EXPORT int keygen_playerAPI(int n, int t, int party_id, int tid[], char* ip_[], int port_[]);
API_EXPORT int keygen_ttpAPI(int n, int t, int tid[], char* ip_[], int port_[]);
API_EXPORT int sign_playerAPI(int n, int t, int party_id, int tid[], char* ip_[], int port_[]);
API_EXPORT int sign_ttpAPI(int n, int t, int tid[], char* ip_[], int port_[]);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net_settings.h"
#include "sign_round.h"
#include "keygen_round.h"
#include "Json.h"

BIGNUM* prime;
int threshold[SPX_D - 1];
pthread_barrier_t barrier;

int keygen_playerAPI(int n, int t, int party_id, int tid[], char* ip_[], int port_[]) {
    if (n > SPX_D - 1 || n < 0) {
        printf("[P%d] n must be in [0, d)\n", party_id);
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("[P%d] t must be in (0, n)\n", party_id);
        return NULL;
    }

    prime = BN_new();
    BN_dec2bn(&prime, P);

    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));

    ctx->n = n;
    ctx->t = t;
    ctx->party_id = party_id;

    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx, n, t - 1);
    ctx->sss_ctx = sss_ctx;



    for (int i = 0; i < t; i++) {
        threshold[i] = tid[i];
    }

    for (int i = 0; i <= n; i++) {
        memcpy(ctx->ip_[i], ip_[i], strlen(ip_[i]) + 1);
        ctx->port_[i] = port_[i];
    }

    ctx->local_ip = malloc(strlen(ctx->ip_[ctx->party_id]) + 1);
    memcpy(ctx->local_ip, ctx->ip_[ctx->party_id], strlen(ctx->ip_[ctx->party_id]) + 1);
    ctx->port = ctx->port_[ctx->party_id];

    
    //执行keygen_round
    if (keygen_round_player(ctx)) {
        printf("[P%d] keygen failed\n", ctx->party_id);   
    }

    // 把keygen生成的关键数据输出(写入文件)
    char filename[256];
    snprintf(filename, sizeof(filename), "data/Web_party_%d_keygen_res.json", ctx->party_id);    
    if (save_ctx_to_file(ctx, filename)) {
        printf("[P%d] Failed to save context\n", ctx->party_id);
    } else {
        printf("[P%d] Results saved successfully\n", ctx->party_id);
    }


    return 0;
}

int keygen_ttpAPI(int n, int t, int tid[], char* ip_[], int port_[]) {
    if (n > SPX_D - 1 || n < 0) {
        printf("[P%d] n must be in [0, d)\n", 0);
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("[P%d] t must be in (0, n)\n", 0);
        return NULL;
    }

    prime = BN_new();
    BN_dec2bn(&prime, P);

    //初始化上下文参数
    KeygenNet_ctx* ctx = (KeygenNet_ctx*)malloc(sizeof(KeygenNet_ctx));

    ctx->n = n;
    ctx->t = t;
    ctx->party_id = 0;

    SSS_ctx* sss_ctx = (SSS_ctx*)malloc(sizeof(SSS_ctx));
    SSS_init(sss_ctx, n, t - 1);
    ctx->sss_ctx = sss_ctx;


    for (int i = 0; i < t; i++) {
        threshold[i] = tid[i];
    }

    for (int i = 0; i <= n; i++) {
        memcpy(ctx->ip_[i], ip_[i], strlen(ip_[i]) + 1);
        ctx->port_[i] = port_[i];
    }

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, n + 1);

    ctx->local_ip = malloc(strlen(ctx->ip_[0]) + 1);
    memcpy(ctx->local_ip, ctx->ip_[0], strlen(ctx->ip_[0]) + 1);
    ctx->port = ctx->port_[0];

    //执行keygen_round
    if (keygen_round_ttp(ctx)) {
        printf("[P%d] keygen failed\n", ctx->party_id);   
    }

    return 0;
}

int sign_playerAPI(int n, int t, int party_id, int tid[], char* ip_[], int port_[]) {
    if (n > SPX_D - 1 || n < 0) {
        printf("[P%d] n must be in [0, d)\n", party_id);
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("[P%d] t must be in (0, n)\n", party_id);
        return NULL;
    }

    prime = BN_new();
    BN_dec2bn(&prime, P);

    // 从文件加载
    SignNet_ctx* ctx = (SignNet_ctx*)malloc(sizeof(SignNet_ctx));
    ctx->party_id = party_id;
    ctx->n = n;
    ctx->t = t;

    const char filename[256];
    snprintf(filename, sizeof(filename), "data/Web_party_%d_keygen_res.json", ctx->party_id);
    int ret = load_ctx_from_file(ctx, filename);
    if (ret) {
        printf("[P%d] Failed to load context\n", ctx->party_id);
        return 1;
    }

    ctx->mlen = MLEN;
    ctx->m = malloc(ctx->mlen);
    memcpy(ctx->m, M, ctx->mlen);

    for (int i = 0; i < t; i++) {
        threshold[i] = tid[i];
    }

    for (int i = 0; i <= n; i++) {
        memcpy(ctx->ip_[i], ip_[i], strlen(ip_[i]) + 1);
        ctx->port_[i] = port_[i];
    }

    //执行sign_round
    if (sign_round_player(ctx)) {
        printf("[P%d] sign failed\n", ctx->party_id);   
    }

    ctx->sm -= ctx->smlen;
    int spx_bytes = (SPX_N + SPX_FORS_BYTES + (t + 1) * SPX_WOTS_BYTES + \
        (t + 1) * SPX_TREE_HEIGHT * SPX_N);

    memmove(ctx->sm + spx_bytes, ctx->m, ctx->mlen);

    // 把sign生成的关键数据输出(写入文件)
    snprintf(filename, sizeof(filename), "data/Web_party_%d_sig.json", ctx->party_id);    
    if (save_sig_to_file(ctx, filename, spx_bytes)) {
        printf("[P%d] Failed to save context\n", ctx->party_id);
    } else {
        printf("[P%d] Results saved successfully\n", ctx->party_id);
    }

    if (tss_crypto_sign_verify(ctx->sm, spx_bytes, ctx->sm + spx_bytes, ctx->mlen, ctx->pk, ctx->t)) {
        printf("[P%d] vrfy failed\n", ctx->party_id);
        return -1;
    }
    printf("[P%d] vrfy success!\n", ctx->party_id);


    return 0;
}

int sign_ttpAPI(int n, int t, int tid[], char* ip_[], int port_[]) {
    if (n > SPX_D - 1 || n < 0) {
        printf("[P%d] n must be in [0, d)\n", 0);
        return NULL;
    }
    if (t >= n || t <= 0) {
        printf("[P%d] t must be in (0, n)\n", 0);
        return NULL;
    }

    prime = BN_new();
    BN_dec2bn(&prime, P);
    //初始化屏障
    pthread_barrier_init(&barrier, NULL, t + 1);

    //初始化上下文参数
    SignNet_ctx* ctx = (SignNet_ctx*)malloc(sizeof(SignNet_ctx));
    ctx->party_id = 0;
    ctx->n = n;
    ctx->t = t;

    for (int i = 0; i < t; i++) {
        threshold[i] = tid[i];
    }

    for (int i = 0; i <= n; i++) {
        memcpy(ctx->ip_[i], ip_[i], strlen(ip_[i]) + 1);
        ctx->port_[i] = port_[i];
    }
    
    ctx->local_ip = malloc(strlen(ctx->ip_[0]) + 1);
    memcpy(ctx->local_ip, ctx->ip_[0], strlen(ctx->ip_[0]));
    ctx->port = ctx->port_[0];

    ctx->mlen = MLEN;
    ctx->m = malloc(ctx->mlen);
    memcpy(ctx->m, M, ctx->mlen);

    //执行sign_round
    if (sign_round_ttp(ctx)) {
        printf("[P%d] sign failed\n", ctx->party_id);   
    }

    return 0;
}