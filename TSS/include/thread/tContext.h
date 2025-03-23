#include <pthread.h>
#include "params.h"
#include "ThreadSafeQueue.h"
#include "VSS.h"
#define SPX_MLEN 32
#ifndef TCONTEXT
#define TCONTEXT
 

typedef struct {
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];
    unsigned char *m;
    unsigned char *sm;
    unsigned char *mout;
    unsigned long long smlen;
    unsigned long long mlen;
    unsigned int tid;
    ThreadSafeQueue *self_channel;
    ThreadSafeQueue* public_channel_list;
    VSS_ctx *vss_ctx;
    
} thread_ctx;

//初始化线程上下文
int init_ctx(thread_ctx* ctx, int tid, unsigned char* M, ThreadSafeQueue* channel, ThreadSafeQueue* public_channel_list);

#endif