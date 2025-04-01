#include "sign_handler.h"

static int send_bignum(int sock, const BIGNUM *num) {
    if (!num) return -1;

    int len = BN_num_bytes(num);
    unsigned char *data = malloc(len);
    if (!data) {
        perror("malloc failed");
        return -1;
    }

    BN_bn2bin(num, data);
    int total_sent = 0;
    while (total_sent < len) {
        int sent = send(sock, data + total_sent, len - total_sent, 0);
        if (sent <= 0) {
            perror("send failed");
            SAFE_FREE(data);
            return -1;
        }
        total_sent += sent;
    }
    SAFE_FREE(data);
    return 0;
}

static int recv_bignum(int sock, BIGNUM* dest) {
    // 动态分配缓冲区
    unsigned char *buf = malloc(BUFFER_SIZE);
    if (!buf) {
        perror("malloc failed");
        return -1;
    }

    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    // 接收数据
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }

    // 反序列化 BIGNUM
    if (!BN_bin2bn(buf, len, dest)) {
        fprintf(stderr, "BN_bin2bn failed\n");
        SAFE_FREE(buf);
        return -1;
    }
    SAFE_FREE(buf);
    return 0;
}

int player_conn_to_ttp(SignNet_ctx* ctx, int sock, int srv_id) {
    if (!ctx || srv_id < 0) return -1;

    BN_CTX* BNctx = BN_CTX_new();

    // 发送共享份额
    if (send_bignum(sock, ctx->share) != 0) {
        close(sock);
        return -1;
    }

    //接收R
    unsigned char *buf = malloc(BUFFER_SIZE);
    if (!buf) {
        perror("malloc failed");
        return -1;
    }

    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    // 接收数据
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }
    ctx->R = malloc(len);
    memcpy(ctx->R, buf, len);
    
    //接收FORS私钥种子
    len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    // 接收数据
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }
    memcpy(ctx->fors_seed, buf, len);


    BN_CTX_free(BNctx);
    close(sock);
    return 0;
}


int ttp_listen_player(SignNet_ctx* ctx, int sock, int srv_id) {
    if (!ctx || srv_id < 0) return -1;

    // 接收共享份额
    if (recv_bignum(sock, ctx->tmp_shares[srv_id]) != 0) {
        close(sock);
        return -1;
    }

    //第一次线程同步，表示此时其余线程均收到拉格朗日共享份额，aggregate_seed线程可以执行了
    pthread_barrier_wait(&barrier);
    //第二次线程同步，表示aggregate_seed线程已成功计算出R和FORS私钥，接下来各个连接可以发送了
    pthread_barrier_wait(&barrier);

    // 发送R
    int len = strlen(ctx->R);
    int sent = send(sock, ctx->R, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    // 发送FORS私钥种子
    len = strlen(ctx->fors_seed);
    sent = send(sock, ctx->fors_seed, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }
    return 0;
}

//聚合种子，计算出R和FORS私钥
void aggregate_seed(SignNet_ctx* ctx) {
    pthread_barrier_wait(&barrier);

    //聚合得到私钥（种子）
    BIGNUM* sum = BN_new();
    BN_CTX* BNctx = BN_CTX_new();
    BN_zero(sum);
    for (int i = 1;i <= PLAYERS;i++) {
        BN_mod_add(sum, sum, ctx->tmp_shares[i], prime, BNctx);
    }
    unsigned char* seed = (unsigned char*)malloc(BN_num_bytes(sum));
    int len = BN_bn2bin(sum, seed);
    if (len <= 0) {
        fprintf(stderr, "Error: Failed to gen seed\n");
    }
    tss_crypto_sign_keypair(ctx->pk, ctx->sk, seed);

    //生成随机数R
    ctx->R = malloc(SPX_BYTES);
    tss_gen_R(ctx->R, ctx->m, ctx->mlen, ctx->sk);

    //生成FORS的私钥种子
    tss_gen_FORS_seed(ctx->fors_seed);

    //计算对顶层签名所需要的地址
    tss_gen_ttp_addr(ctx->pk, ctx->mhash, ctx->m, ctx->mlen,
        &ctx->tree, &ctx->idx_leaf, ctx->wots_addr, ctx->tree_addr, ctx->R);


    BN_CTX_free(BNctx);
    BN_free(sum);

    pthread_barrier_wait(&barrier);
    pthread_detach(pthread_self());
}