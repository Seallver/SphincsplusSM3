#include "communication.h"

// 封装消息
Msg* Pack_Msg(int from, int to, void* data) {
    Msg* msg = (Msg*)malloc(sizeof(Msg));
    msg->from = from;
    msg->to = to;
    msg->data = data;
    msg->next = NULL;
    return msg;
}

// 计算 BIGNUM** 序列化后的大小
size_t BIGNUM_SerializeSize(BIGNUM** comms, int threshold) {
    size_t size = 0;
    for (int i = 0; i < threshold; i++) {
        size += BN_num_bytes(comms[i]); // 每个 BIGNUM 的大小
    }
    return size + threshold * sizeof(int); // 额外存储每个 BIGNUM 的大小
}

// 序列化 BIGNUM**
unsigned char* BIGNUM_Serialize(BIGNUM** comms, int threshold) {
    size_t total_size = BIGNUM_SerializeSize(comms, threshold);
    unsigned char* buffer = (unsigned char*)malloc(total_size);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    unsigned char* ptr = buffer;
    for (int i = 0; i < threshold; i++) {
        int size = BN_num_bytes(comms[i]);
        memcpy(ptr, &size, sizeof(int)); // 存储 BIGNUM 的大小
        ptr += sizeof(int);

        BN_bn2bin(comms[i], ptr); // 存储 BIGNUM 的数据
        ptr += size;
    }

    return buffer;
}

// 反序列化 BIGNUM**
BIGNUM** BIGNUM_Deserialize(const unsigned char* buffer, int threshold) {
    BIGNUM** comms = (BIGNUM**)malloc(threshold * sizeof(BIGNUM*));
    if (!comms) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    const unsigned char* ptr = buffer;
    for (int i = 0; i < threshold; i++) {
        int size;
        memcpy(&size, ptr, sizeof(int)); // 读取 BIGNUM 的大小
        ptr += sizeof(int);

        comms[i] = BN_new();
        BN_bin2bn(ptr, size, comms[i]); // 读取 BIGNUM 的数据
        ptr += size;
    }

    return comms;
}

// 发送消息
void Send_Msg(ThreadSafeQueue* queuelist, int from, int to, void* data) {
    Msg* msg = Pack_Msg(from, to, data);
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
void Recv_Msg(ThreadSafeQueue* queue, int* party_id, void** data) {
    Msg* msg = ThreadSafeQueue_Pop(queue);
    *party_id = msg->from;
    *data = msg->data;
}