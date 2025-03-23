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