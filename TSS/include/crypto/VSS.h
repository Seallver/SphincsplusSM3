#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/bn.h>
#include "params.h"


// 质数相关参数
#define MIN_PRIME_BITS SPX_N * 8   // 最小质数位数
#define MR_TEST_ROUNDS 5    // Miller-Rabin测试轮数

#define PARTY_SEEDBYTES 8  // 参与方种子(秘密)长度
#define RANDOM_NUM_BYTES 1  //uint64_t的随机数最多8个字节

typedef struct
{
    BIGNUM *p; // 质数
    BIGNUM *g; // 原根
    BIGNUM* secret; // 秘密
    BIGNUM** shares; // 分享
    BIGNUM** comms; // 承诺
    BIGNUM** coeffs; // 多项式系数
    int degree; // 多项式次数
} VSS_ctx;

// 初始化VSS上下文，各参与方保持质数、原根、多项式次数一致，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx, BIGNUM* p, BIGNUM* g);


// 查找原根
BIGNUM* find_primitive_root(BIGNUM* p, BN_CTX* ctx);

// 初始化加密参数
void init_crypto_params(BIGNUM* PRIME, BIGNUM* GENERATOR);

// 生成多项式系数（模PRIME-1）
void generate_coefficients(BIGNUM** coeffs, int degree, const BIGNUM *secret, const BIGNUM *prime);

// 多项式求值
void evaluate_poly(BIGNUM *result, BIGNUM** coeffs, int degree, const BIGNUM *x, const BIGNUM *prime);

// 生成承诺
void generate_commitments(BIGNUM **comms, BIGNUM** coeffs, int degree,const BIGNUM *prime,const BIGNUM *g);

// 验证分享
int verify_share(const BIGNUM *share, const BIGNUM *x, const BIGNUM **comms, int degree, const BIGNUM *prime,const BIGNUM *g);