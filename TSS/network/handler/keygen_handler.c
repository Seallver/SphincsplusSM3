#include "keygen_handler.h"



// 发送 BIGNUM 数据（返回 0 成功，-1 失败）
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

// 接收 BIGNUM 数据（返回 0 成功，-1 失败）
static int recv_bignum(int sock, BIGNUM *dest) {
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

int ttp_handler_recv(KeygenNet_ctx* ctx, int sock, int srv_id) {
    if (recv_bignum(sock, ctx->tmp_shares[srv_id]) != 0) {
        close(sock);
        return -1;
    }
    // printf("%s\n", BN_bn2hex(ctx->tmp_shares[srv_id]));
    //第一次线程同步，表示此时其余线程均收到私钥份额，complete_pk线程可以执行了
    pthread_barrier_wait(&barrier);
    //第二次线程同步，表示complete_pk线程已成功计算出公钥，接下来各个连接可以发送公钥了
    pthread_barrier_wait(&barrier);

    int sent = send(sock, ctx->pk, SPX_PK_BYTES, 0);
    if (sent <= 0) {
        perror("send failed");
        return -1;
    }

    return 0;
}

int player_handler_recv(KeygenNet_ctx* ctx, int sock, int srv_id) {
    // 1. 接收共享份额
    BIGNUM* my_shares = BN_new();
    if (recv_bignum(sock, my_shares) != 0) {
        close(sock);
        return -1;
    }

    // 2. 发送共享份额
    if (send_bignum(sock, ctx->tmp_shares[srv_id]) != 0) {
        close(sock);
        return -1;
    }
    BN_copy(ctx->tmp_shares[srv_id], my_shares);


    // 3. 接收随机数
    BIGNUM * tmp = BN_new();
    if (recv_bignum(sock, tmp) != 0) {
        close(sock);
        return -1;
    }

    // 4. 发送随机数
    if (send_bignum(sock, ctx->sss_ctx->random_list[srv_id]) != 0) {
        close(sock);
        return -1;
    }

    BN_mod_add(ctx->sss_ctx->random_list[srv_id], ctx->sss_ctx->random_list[srv_id], tmp, prime, BN_CTX_new());

    // printf("%s\n", BN_bn2hex(ctx->sss_ctx->random_list[srv_id]));

    BN_free(my_shares);
    BN_free(tmp);
    return 0;
}

int player_handler_send(KeygenNet_ctx* ctx, int sock, int srv_id) {
    if (!ctx || srv_id < 0) return -1;

    // 1. 发送共享份额
    if (send_bignum(sock, ctx->tmp_shares[srv_id]) != 0) {
        close(sock);
        return -1;
    }

    // 2. 接收共享份额
    if (recv_bignum(sock, ctx->tmp_shares[srv_id]) != 0) {
        close(sock);
        return -1;
    }

    // 3. 发送随机数
    if (send_bignum(sock, ctx->sss_ctx->random_list[srv_id]) != 0) {
        close(sock);
        return -1;
    }

    // 4. 接收随机数
    BIGNUM * tmp = BN_new();
    if (recv_bignum(sock, tmp) != 0) {
        close(sock);
        return -1;
    }

    BN_mod_add(ctx->sss_ctx->random_list[srv_id], ctx->sss_ctx->random_list[srv_id], tmp, prime, BN_CTX_new());

    // printf("%s\n", BN_bn2hex(ctx->sss_ctx->random_list[srv_id]));

    BN_free(tmp);
    close(sock);
    return 0;
}


int player_handler_send_ttp(KeygenNet_ctx* ctx, int sock, int srv_id) {
    if (!ctx || srv_id < 0) return -1;

    BN_CTX* BNctx = BN_CTX_new();
    
    BIGNUM* blinding_shard = BN_new();
    BN_copy(blinding_shard, ctx->sss_ctx->secret);
    for (int i = 1;i <= ctx->n;i++) {
        if (i < ctx->party_id) {
            BN_mod_add(blinding_shard, blinding_shard, ctx->sss_ctx->random_list[i], prime, BNctx);
        }
        if (i > ctx->party_id) {
            BN_mod_sub(blinding_shard, blinding_shard, ctx->sss_ctx->random_list[i], prime, BNctx);
        }
    }

    // printf("%s\n", BN_bn2hex(blinding_shard));

    if (send_bignum(sock, blinding_shard) != 0) {
        close(sock);
        return -1;
    }

    unsigned char* buf = malloc(BUFFER_SIZE);
    if (!buf){
        perror("malloc failed");
        return -1;
    }

    int len = recv(sock, buf, BUFFER_SIZE - 1, 0);
    if (len != SPX_PK_BYTES) {
        perror("recv data failed");
        SAFE_FREE(buf);
        return -1;
    }

    memcpy(ctx->pk, buf, len);


    BN_CTX_free(BNctx);
    BN_free(blinding_shard);
    SAFE_FREE(buf);
    close(sock);
    return 0;
}

void complete_pk(KeygenNet_ctx* ctx) {
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

    BN_CTX_free(BNctx);
    BN_free(sum);
    pthread_barrier_wait(&barrier);
    pthread_detach(pthread_self());
}