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
    memcpy(ctx->sm, buf, SPX_BYTES);

    //接收FORS私钥种子
    len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    // 接收数据
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }
    memcpy(ctx->sk, buf, len);

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
    int len = SPX_N;
    int sent = send(sock, ctx->R, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    // 发送FORS私钥种子
    len = SPX_N;
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
    for (int i = 1;i <= ctx->n;i++) {
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

    memset(ctx->wots_addr, 0, sizeof(ctx->wots_addr));
    memset(ctx->tree_addr, 0, sizeof(ctx->tree_addr));

    //计算对顶层签名所需要的地址
    tss_gen_ttp_addr(ctx->pk, ctx->mhash, ctx->m, ctx->mlen,
        &ctx->tree, &ctx->idx_leaf, ctx->wots_addr, ctx->tree_addr, ctx->R);
    
    BN_CTX_free(BNctx);
    BN_free(sum);

    pthread_barrier_wait(&barrier);
    pthread_detach(pthread_self());
}

int send_root(SignNet_ctx* ctx, int sock, int srv_id) {
    if (!ctx || srv_id < 0) return -1;

    int len = SPX_N;
    int sent = send(sock, ctx->root, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    // printf("root:\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%02x", ctx->root[i]);
    // }
    // printf("\n");

    close(sock);
    return 0;
}


int recv_root(SignNet_ctx* ctx, int sock, int srv_id) {
    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }

    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }

    // printf("root:\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%02x", buf[i]);
    // }
    // printf("\n");

    memcpy(ctx->root, buf, len);
    SAFE_FREE(buf);
    return 0;
}

int ttp_recv_root(SignNet_ctx* ctx, int sock, int srv_id) {
    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }

    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }


    // printf("root:\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%02x", buf[i]);
    // }
    // printf("\n");

    memcpy(ctx->root, buf, len);
    SAFE_FREE(buf);
    return 0;
}


int bc_top_sig(SignNet_ctx* ctx, int sock, int srv_id) {
    int len = SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    int sent = send(sock, ctx->sig_shard, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }


    // printf("sig_shard:\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%02x", ctx->sig_shard[i]);
    // }
    // printf("\n");


    return 0;
}

int conn_exchange_sig(SignNet_ctx* ctx, int sock, int srv_id) {
    //先发送自己的签名份额
    int len = SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    int sent = send(sock, ctx->sig_shard, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    //再接受并聚合对方发来的签名份额
    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }

    len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }
    int index;
    for (int i = 0;i < ctx->t;i++)
        if (threshold[i] == srv_id)
            index = i;
    ctx->sm += (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index;
    memcpy(ctx->sm, buf, len);
    ctx->sm -= (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index;
    ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;


    close(sock);
    SAFE_FREE(buf);
    return 0;
}

int listen_exchange_sig(SignNet_ctx* ctx, int sock, int srv_id) {
    //先接收对方的签名份额并聚合
    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }
    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }
    int index;
    for (int i = 0;i < ctx->t;i++)
        if (threshold[i] == srv_id)
            index = i;
    ctx->sm += (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index;
    memcpy(ctx->sm, buf, len);
    ctx->sm -= (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index;
    ctx->smlen += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    
    //发送自己的签名份额
    len = SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    int sent = send(sock, ctx->sig_shard, len, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    SAFE_FREE(buf);
    return 0;
}

int final_sig(SignNet_ctx* ctx, int sock, int srv_id) {
    //接收对方的签名份额并聚合
    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }
    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len == 0) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }

    // printf("sig_shard:\n");
    // for(int i = 0; i < len; i++) {
    //     printf("%02x", buf[i]);
    // }
    // printf("\n");

    ctx->sm += (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * (ctx->t);
    memcpy(ctx->sm, buf, len);
    ctx->sm -= (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * (ctx->t);

    close(sock);
    SAFE_FREE(buf);
    return 0;
}