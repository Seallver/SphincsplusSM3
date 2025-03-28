#include "tContext.h"

//参与方p2p发送shares
void keygen_player_p2p_shares(thread_ctx* thread_ctx, BIGNUM** shares);

//参与方接收share_shards，并聚合成share
void keygen_player_recv_shares(thread_ctx* thread_ctx);

//参与方向TTP发送shards
void keygen_player_p2ttp_shards(thread_ctx* thread_ctx, BIGNUM* shards);

// TTP接收shards,并聚合成私钥
void keygen_ttp_recv_shards(thread_ctx* thread_ctx, BIGNUM* sk);

// TTP广播pk
void keygen_ttp_bc_pk(thread_ctx* thread_ctx, const unsigned char* pk);

//参与方接收pk
void keygen_recv_pk(thread_ctx* thread_ctx);

//参与方向TTP发送lagrange份额
void presign_player_p2ttp_shards(thread_ctx* ctx);

//TTP接收lagrange份额
void ttp_recv_lagrange_shards(thread_ctx* thread_ctx, BIGNUM* sk);

//TTP广播R
void presign_ttp_bc_R(thread_ctx* thread_ctx, unsigned char* R);

//TTP广播FORS私钥种子
void presign_ttp_bc_FORS_seed(thread_ctx* thread_ctx, unsigned char* seed);

//参与方接收R
void presign_player_recv_R(thread_ctx* thread_ctx);

//参与方接收FORS私钥种子
void presign_player_recv_seed(thread_ctx* ctx);

//门限方广播签名份额
void sign_bc_sig_shards(thread_ctx*ctx, const unsigned char *sig_shards);

//门限方接收签名份额
void sign_recv_sig_shards(thread_ctx* ctx, unsigned char* sig_shards);

//门限方向下一个门限层发送root
void sign_p2p_root(thread_ctx* ctx, int to);

//门限方接收root
void sign_recv_root(thread_ctx* ctx);

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

