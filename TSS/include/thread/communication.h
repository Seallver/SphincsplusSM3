#include "ThreadSafeQueue.h"
#include "VSS.h"
#include "tContext.h"

// TTP发送shares
void p2p_shares(TTP_VSS_ctx* vss_ctx, ThreadSafeQueue * channel);

// TTP广播comms
void bc_comms(TTP_VSS_ctx* vss_ctx, ThreadSafeQueue* channel);

// TTP广播pk
void bc_pk(thread_ctx* thread_ctx);

//参与方接收shares
void recv_shares(thread_ctx* thread_ctx);

//参与方接收comms
void recv_comms(thread_ctx* thread_ctx);

//参与方接收pk
void recv_pk(thread_ctx* thread_ctx);

// 发送消息(小于0的时候认为是广播)
void Send_Msg(ThreadSafeQueue* queue, int from, int to, const unsigned char* data, size_t data_len);

// 封装消息
Msg* Pack_Msg(int from, int to, void* data, size_t data_len);

// 广播消息(不会对自己和TTP广播)
void Broadcast_Msg(ThreadSafeQueue* queuelist, Msg* msg, int from);

// P2P传递消息
void P2P_Msg(ThreadSafeQueue* queue, Msg * msg);

// 接收消息
void Recv_Msg(ThreadSafeQueue* queue, Msg* recvmsg);