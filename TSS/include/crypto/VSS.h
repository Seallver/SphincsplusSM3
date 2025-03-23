#ifndef VSS_H
#define VSS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/bn.h>
#include "params.h"


// 质数相关参数
#define MIN_PRIME_BITS SPX_N * 8   // 最小质数位数
#define MR_TEST_ROUNDS 5    // Miller-Rabin测试轮数


typedef struct
{
    BIGNUM* p; // 质数
    BIGNUM* g; // 原根
    BIGNUM* share; // 秘密份额
    BIGNUM** comms; // 承诺
} VSS_ctx;

typedef struct
{
    BIGNUM* p; // 质数
    BIGNUM* g; // 原根
    BIGNUM* secret; // 秘密
    BIGNUM** comms; // 承诺
    BIGNUM** shares;  // 秘密份额
    BIGNUM** coeffs;  // 多项式系数

    int degree; // 多项式次数
} TTP_VSS_ctx;

// 初始化VSS上下文
void VSS_init(VSS_ctx* ctx, BIGNUM* p, BIGNUM* g);

// 初始化TTP_VSS上下文
void TTP_VSS_init(TTP_VSS_ctx* ctx, BIGNUM* P, BIGNUM* G);

//生成随机秘密（也是SPX的私钥）
void generate_secret(BIGNUM* secret, BIGNUM* p);

// 查找原根
BIGNUM* find_primitive_root(BIGNUM* p, BN_CTX* ctx);

// 初始化加密参数
void init_crypto_params(BIGNUM* PRIME, BIGNUM* GENERATOR);

// 生成多项式系数（模PRIME-1）
void generate_coefficients(BIGNUM** coeffs, int degree, const BIGNUM *secret, const BIGNUM *prime, BN_CTX *ctx);

// 多项式求值
void evaluate_poly(BIGNUM *result, BIGNUM** coeffs, int degree, const BIGNUM *x, const BIGNUM *prime, BN_CTX *ctx);

// 生成承诺
void generate_commitments(BIGNUM** comms, BIGNUM** coeffs, int degree, const BIGNUM* prime, const BIGNUM* g, BN_CTX *ctx);

// 生成秘密份额
void generate_shares(BIGNUM** shares, BIGNUM** coeffs, int degree,const BIGNUM* p, BN_CTX *ctx);

// 验证分享
int verify_share(const BIGNUM* share, const BIGNUM* x, const BIGNUM** comms, int degree, const BIGNUM* p, const BIGNUM* g);

#endif