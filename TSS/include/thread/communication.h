#include "ThreadSafeQueue.h"
#include "params.h"

// 发送消息(小于0的时候认为是广播)
void Send_Msg(ThreadSafeQueue* queue, int from, int to, void* data);

// 封装消息
Msg* Pack_Msg(int from, int to, void* data);

// 广播消息(不会对自己和TTP广播)
void Broadcast_Msg(ThreadSafeQueue* queuelist, Msg* msg, int from);

// P2P传递消息
void P2P_Msg(ThreadSafeQueue* queue, Msg * msg);

// 接收消息
void Recv_Msg(ThreadSafeQueue* queue, int *party_id, void** data);