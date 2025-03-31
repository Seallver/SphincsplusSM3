#ifndef TCONTEXT
#define TCONTEXT

#include <pthread.h>
#include "ThreadSafeQueue.h"
#include "SSS.h"

typedef struct {
    unsigned int tid;

    //公钥和私钥份额
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];

    //签名用到的变量
    unsigned char* m;
    unsigned char* sm;
    unsigned char* mout;
    unsigned long long smlen;
    unsigned long long mlen;
    unsigned char mhash[SPX_FORS_MSG_BYTES];
    unsigned char root[SPX_N];

    //签名用到的地址编码
    uint32_t wots_addr[8];
    uint32_t tree_addr[8];
    uint64_t tree;
    uint32_t idx_leaf;

    //信道
    ThreadSafeQueue* self_channel;
    ThreadSafeQueue* public_channel_list;

    //SSS上下文
    SSS_ctx* sss_ctx;  //for player , 无记忆 TTP 无法保留参数
} thread_ctx;

//初始化线程上下文
int init_ctx(thread_ctx* ctx, int tid, unsigned long long mlen,ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list);

//签名的时候输入明文
int input_m(thread_ctx* ctx, unsigned char* M);


#endif