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

    //SSS过程的中间变量
    BIGNUM** tmp_shares;
    BIGNUM* tmp_seed;

} KeygenNet_ctx;

typedef struct {
    int party_id;
    char* local_ip;
    int port;

    //公钥和私钥份额
    unsigned char pk[SPX_PK_BYTES];
    unsigned char sk[SPX_SK_BYTES];

    unsigned char* m;
    unsigned char* sm;
    unsigned long long smlen;
    unsigned long long mlen;
    unsigned char mhash[SPX_FORS_MSG_BYTES];
    unsigned char root[SPX_N];

    uint32_t wots_addr[8];
    uint32_t tree_addr[8];
    uint64_t tree;
    uint32_t idx_leaf;

    BIGNUM* secret;
    BIGNUM* share;

    //SSS过程的中间变量
    BIGNUM** tmp_shares;

    unsigned char* R;
    unsigned char fors_seed[SPX_N];

} SignNet_ctx;




#endif