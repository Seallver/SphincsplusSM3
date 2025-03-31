#ifndef NET_CONTEXT
#define NET_CONTEXT

#include "SSS.h"


typedef struct {
    //net信息
    int party_id;
    char* local_ip;
    int port;

    //公钥和私钥份额
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];

    //SSS上下文
    SSS_ctx* sss_ctx; 

} KeygenNet_ctx;



#endif