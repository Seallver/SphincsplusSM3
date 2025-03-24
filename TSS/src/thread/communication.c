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

//TTP发送shares
void p2p_shares(TTP_VSS_ctx* vss_ctx, ThreadSafeQueue* channel) {
    for (int i = 1; i <= PLAYERS; i++) {
        BIGNUM* share = vss_ctx->shares[i]; // 获取 shares[i]
        if (!share) {
            fprintf(stderr, "Error: shares[%d] is NULL\n", i);
            continue;
        }

        // 将 BIGNUM 序列化为字节数组
        size_t data_len = BN_num_bytes(share); // 计算 BIGNUM 的字节长度
        unsigned char* data = (unsigned char*)malloc(data_len); // 分配内存
        if (!data) {
            fprintf(stderr, "Error: Failed to allocate memory for data\n");
            continue;
        }

        // 将 BIGNUM 转换为字节数组
        int len = BN_bn2bin(share, data); // 返回实际写入的字节数
        if (len <= 0) {
            fprintf(stderr, "Error: Failed to serialize BIGNUM\n");
            free(data);
            continue;
        }

        // 发送数据
        Send_Msg(channel, 0, i, data, data_len);

        // 释放内存
        free(data);
    }
}

//TTP广播comms
void bc_comms(TTP_VSS_ctx* vss_ctx, ThreadSafeQueue * channel) {
    for (int i = 0;i < vss_ctx->degree + 1;i++) {
        BIGNUM* comms =  vss_ctx->comms[i];
        if (!comms) {
            fprintf(stderr, "Error: comms[%d] is NULL\n", i);
            continue;
        }

        size_t data_len = BN_num_bytes(comms);
        unsigned char* data = (unsigned char*)malloc(data_len);
        if (!data) {
            fprintf(stderr, "Error: Failed to allocate memory for data\n");
            continue;
        }

        int len = BN_bn2bin(comms, data); // 返回实际写入的字节数
        if (len <= 0) {
            fprintf(stderr, "Error: Failed to serialize BIGNUM\n");
            free(data);
            continue;
        }

        Send_Msg(channel, 0, -1, data, data_len);
    }
}

//TTP广播pk
void bc_pk(thread_ctx* thread_ctx) {
    unsigned char pk[SPX_PK_BYTES];
    memcpy(pk, thread_ctx->pk, SPX_PK_BYTES);
    Send_Msg(thread_ctx->public_channel_list, 0, -1, &pk, SPX_PK_BYTES);
}


//参与方接收shares
void recv_shares(thread_ctx* thread_ctx) {
    Msg* shares = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(thread_ctx->self_channel, shares);
    unsigned char* data = shares->data;
    size_t data_len = shares->data_len;
    BIGNUM* bn = BN_new();
    // 将字节数组转换为 BIGNUM
    if (!BN_bin2bn(data, data_len, bn)) {
        fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
        BN_free(bn); // 释放 BIGNUM
        return NULL;
    }
    BN_copy(thread_ctx->vss_ctx->share, bn);
}


//参与方接收comms
void recv_comms(thread_ctx* thread_ctx) {
    Msg* comms = (Msg*)malloc(sizeof(Msg));
    size_t data_len;
    for (int i = 0; i < THRESHOLD; i++) {
        Recv_Msg(thread_ctx->self_channel, comms);
        data_len = comms->data_len;
        unsigned char* data = comms->data;
        BIGNUM* bn = BN_new();
        if (!BN_bin2bn(data, data_len, bn)) {
            fprintf(stderr, "Error: Failed to deserialize BIGNUM\n");
            BN_free(bn); // 释放 BIGNUM
            return NULL;
        }
        thread_ctx->vss_ctx->comms[i] = BN_new();
        BN_copy(thread_ctx->vss_ctx->comms[i], bn);
    }
}

void recv_pk(thread_ctx* thread_ctx) {
    Msg* PK = (Msg*)malloc(sizeof(Msg));
    Recv_Msg(thread_ctx->self_channel, PK);
    unsigned char* data = PK->data;
    size_t data_len = PK->data_len;
    memcpy(thread_ctx->pk, data, SPX_PK_BYTES);
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
        if (i != from) {
            ThreadSafeQueue_Push(&queuelist[i], msg);
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