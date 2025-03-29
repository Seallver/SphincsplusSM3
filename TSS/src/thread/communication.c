#include "communication.h"

// 封装消息
Msg* Pack_Msg(int from, int to, void* data, size_t data_len) {
    Msg* msg = (Msg*)malloc(sizeof(Msg));
    msg->from = from;
    msg->to = to;
    msg->data = malloc(data_len);
    memcpy(msg->data, data, data_len);
    msg->next = NULL;
    msg->data_len = data_len;
    return msg;
}

void keygen_player_p2p_shares(thread_ctx* thread_ctx, BIGNUM** shares) {
    int from = thread_ctx->tid;
    int to;
    for (int i = 1;i <= PLAYERS;i++) {
        BIGNUM* share = shares[i];
        to = i;
        
        size_t data_len = BN_num_bytes(share);
        unsigned char* data = (unsigned char*)malloc(data_len);
        int len = BN_bn2bin(share, data);

        Send_Msg(thread_ctx->public_channel_list, from, to, data, data_len);
    }
}

void keygen_player_recv_shares(thread_ctx* thread_ctx, BN_CTX* BNctx) {
    BIGNUM** share_shards = (BIGNUM**)malloc(sizeof(BIGNUM*) * PLAYERS);
    for (int i = 0; i < PLAYERS;i++) {
        Msg* shards = (Msg*)malloc(sizeof(Msg));
        share_shards[i] = BN_new();
        Recv_Msg(thread_ctx->self_channel, shards);
        unsigned char* data = shards->data;
        size_t data_len = shards->data_len;
        // 将字节数组转换为 BIGNUM
        if (!BN_bin2bn(data, data_len, share_shards[i])) {
            fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
            BN_free(data); // 释放 BIGNUM
            return NULL;
        }
    }
    aggregate_shares(thread_ctx->sss_ctx->share, share_shards, BNctx);
}

void keygen_randome_agreement(thread_ctx* thread_ctx, BN_CTX* BNctx) {
    int from = thread_ctx->tid;
    int to;
    //发送消息
    for (int i = 1;i <= PLAYERS;i++) {
        to = i;
        if(to == from) continue;
        size_t data_len = BN_num_bytes(thread_ctx->sss_ctx->random_list[i]);
        unsigned char* data = (unsigned char*)malloc(data_len);
        int len = BN_bn2bin(thread_ctx->sss_ctx->random_list[i], data);
        Send_Msg(thread_ctx->public_channel_list, from, to, data, data_len);
    }
    //接收消息
    BIGNUM *rji = BN_new();
    for (int i = 0;i < PLAYERS - 1;i++) {
        Msg* msg = (Msg*)malloc(sizeof(Msg));
        Recv_Msg(thread_ctx->self_channel, msg);
        unsigned char* data = msg->data;
        size_t data_len = msg->data_len;
        from = msg->from;
        // 将字节数组转换为 BIGNUM
        if (!BN_bin2bn(data, data_len, rji)) {
            fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
            BN_free(data); // 释放 BIGNUM
            return NULL;
        }
        BN_mod_add(thread_ctx->sss_ctx->random_list[from], thread_ctx->sss_ctx->random_list[from], rji, prime, BNctx);
    }
}



void keygen_player_p2ttp_shards(thread_ctx* thread_ctx, BIGNUM* shards, BN_CTX* BNctx) {
    int from = thread_ctx->tid;
    int to = 0;
    BIGNUM* blinding_shard = BN_new();
    BN_copy(blinding_shard, shards);
    for (int i = 1;i <= PLAYERS;i++) {
        if (i < thread_ctx->tid) {
            BN_mod_add(blinding_shard, blinding_shard, thread_ctx->sss_ctx->random_list[i], prime, BNctx);
        }
        if (i > thread_ctx->tid) {
            BN_mod_sub(blinding_shard, blinding_shard, thread_ctx->sss_ctx->random_list[i], prime, BNctx);
        }
    }
    size_t data_len = BN_num_bytes(blinding_shard);
    unsigned char* data = (unsigned char*)malloc(data_len);
    int len = BN_bn2bin(blinding_shard, data);
    Send_Msg(thread_ctx->public_channel_list, from, to, data, data_len);
}

void keygen_ttp_recv_shards(thread_ctx* thread_ctx, BIGNUM* sk) {
    BIGNUM** shards = (BIGNUM**)malloc(sizeof(BIGNUM*) * PLAYERS);
    for (int i = 0;i < PLAYERS;i++) {
        Msg* shards_msg = (Msg*)malloc(sizeof(Msg));
        Recv_Msg(thread_ctx->self_channel, shards_msg);
        unsigned char* data = shards_msg->data;
        size_t data_len = shards_msg->data_len;
        // 将字节数组转换为 BIGNUM
        shards[i] = BN_new();
        if (!BN_bin2bn(data, data_len, shards[i])) {
            fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
            BN_free(data); // 释放 BIGNUM
            return NULL;
        }
    }
    recover_secret(sk, shards, PLAYERS);
}

void keygen_ttp_bc_pk(thread_ctx* thread_ctx, const unsigned char* pk) {
    unsigned char PK[SPX_PK_BYTES];
    memcpy(PK, pk, SPX_PK_BYTES);
    Send_Msg(thread_ctx->public_channel_list, 0, -1, &PK, SPX_PK_BYTES);
}

void keygen_recv_pk(thread_ctx* thread_ctx) {
    Msg* PK = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(thread_ctx->self_channel, PK);
    unsigned char* data = PK->data;
    size_t data_len = PK->data_len;
    memcpy(thread_ctx->pk, data, data_len);
}

void presign_player_p2ttp_shards(thread_ctx* ctx) {
    size_t data_len = BN_num_bytes(ctx->sss_ctx->share);
    unsigned char* data = (unsigned char*)malloc(data_len);
    int len = BN_bn2bin(ctx->sss_ctx->share, data);
    Send_Msg(ctx->public_channel_list, ctx->tid, 0, data, data_len);
}

void ttp_recv_lagrange_shards(thread_ctx* thread_ctx, BIGNUM* sk) {
    BIGNUM** shards = (BIGNUM**)malloc(sizeof(BIGNUM*) * THRESHOLD);
    for (int i = 0;i < THRESHOLD;i++) {
        Msg* shards_msg = (Msg*)malloc(sizeof(Msg));
        Recv_Msg(thread_ctx->self_channel, shards_msg);
        unsigned char* data = shards_msg->data;
        size_t data_len = shards_msg->data_len;
        // 将字节数组转换为 BIGNUM
        shards[i] = BN_new();
        if (!BN_bin2bn(data, data_len, shards[i])) {
            fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
            BN_free(data); // 释放 BIGNUM
            return NULL;
        }
    }
    recover_secret(sk, shards, THRESHOLD);
}

void presign_ttp_bc_R(thread_ctx* thread_ctx, unsigned char* R) {
    Send_Msg(thread_ctx->public_channel_list, 0, -1, R, SPX_BYTES);
}

void presign_ttp_bc_FORS_seed(thread_ctx* thread_ctx, unsigned char * seed) {
    Send_Msg(thread_ctx->public_channel_list, 0, -1, seed, SPX_N);
}


void presign_player_recv_R(thread_ctx* thread_ctx) {
    Msg* R = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(thread_ctx->self_channel, R);
    unsigned char* data = R->data;
    size_t data_len = R->data_len;
    memcpy(thread_ctx->sm, data, data_len);
}

void presign_player_recv_seed(thread_ctx* ctx) {
    Msg* seedmsg = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(ctx->self_channel, seedmsg);
    unsigned char* data = seedmsg->data;
    size_t data_len = seedmsg->data_len;
    memcpy(ctx->sk, data, data_len);
}

void sign_bc_sig_shards(thread_ctx* ctx, const unsigned char* sig_shards) {
    Send_Msg(ctx->public_channel_list, ctx->tid, -1, sig_shards, SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);    
}

void sign_recv_sig_shards(thread_ctx* ctx, unsigned char* sig_shards) {
    Msg* msg = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(ctx->self_channel, msg);
    unsigned char* data = msg->data;
    size_t data_len = msg->data_len;
    memcpy(sig_shards, data, data_len);
}

void sign_p2p_root(thread_ctx* ctx, int to) {
    Send_Msg(ctx->public_channel_list, ctx->tid, to, ctx->root, SPX_N);
}

void sign_recv_root(thread_ctx* ctx) {
    Msg* msg = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(ctx->self_channel, msg);
    unsigned char* data = msg->data;
    size_t data_len = msg->data_len;
    memcpy(ctx->root, data, data_len);
}



// 发送消息
void Send_Msg(ThreadSafeQueue* queuelist, int from, int to, const unsigned char* data, size_t data_len) {
    Msg* msg = Pack_Msg(from, to, data, data_len);
    if (to < 0) {
        Broadcast_Msg(queuelist, msg, from);
    } else {
        P2P_Msg(&queuelist[to], msg);
    }
}


// 广播消息(不会对自己和TTP广播)
void Broadcast_Msg(ThreadSafeQueue* queuelist, Msg* msg, int from) {
    for (int i = 1; i < NUMBER_OF_THREADS; i++) {
        Msg* newMsg = (Msg*)malloc(sizeof(Msg));
        *newMsg = *msg;
        if (i != from) {
            ThreadSafeQueue_Push(&queuelist[i], newMsg);
        }
    }
}

// P2P传递消息
void P2P_Msg(ThreadSafeQueue* queue, Msg * msg) {
    ThreadSafeQueue_Push(queue, msg);
}

// 接收消息
void Recv_Msg(ThreadSafeQueue* queue, Msg* recvmsg) {
    Msg* msg = (Msg*)malloc(sizeof(Msg));
    msg = ThreadSafeQueue_Pop(queue);
    *recvmsg = *msg;
}