#include "ThreadSafeQueue.h"
#include "VSS.h"
#include "tContext.h"

// TTP发送shares
void p2p_shares(thread_ctx* thread_ctx);

// TTP广播comms
void keygen_bc_comms(thread_ctx* thread_ctx);

// TTP广播pk
void keygen_bc_pk(thread_ctx* thread_ctx);

//参与方接收shares
void keygen_recv_shares(thread_ctx* thread_ctx);

//参与方接收comms
void keygen_recv_comms(thread_ctx* thread_ctx);

//参与方接收pk
void keygen_recv_pk(thread_ctx* thread_ctx);

//参与方广播自己的shares
void sign_bc_shares(thread_ctx* thread_ctx);

//参与方接收其他参与方的shares
void sign_recv_shares(thread_ctx* thread_ctx, BIGNUM** shares);

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