#ifndef VSS_H
#define VSS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <openssl/bn.h>
#include "params.h"
#include "globals.h"

// 质数相关参数
#define MIN_PRIME_BITS 3 * SPX_N * 8   // 最小质数位数
#define DEGREE THRESHOLD - 1

typedef struct
{
    BIGNUM* secret; // 秘密份额(即0次项)
    BIGNUM** coeffs; // 多项式系数
    BIGNUM* share; // 共享份额
    BIGNUM** random_list; // 随机数列表
} VSS_ctx;

// 初始化VSS上下文，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx);

//生成随机秘密
void generate_secret(BIGNUM* secret);

// 初始化加密参数
void init_crypto_params(BIGNUM* PRIME);

// 生成多项式系数
void generate_coefficients(BIGNUM** coeffs, const BIGNUM *secret,  BN_CTX *ctx);

// 多项式求值
void evaluate_poly(BIGNUM *result, BIGNUM** coeffs,const BIGNUM *x,  BN_CTX *ctx);

// 生成共享份额，即i发送给j的份额f_i(j)
void generate_shares(BIGNUM** shares, BIGNUM** coeffs,  BN_CTX *ctx);

//聚合共享份额，计算出y_i
void aggregate_shares(BIGNUM* shares, BIGNUM** shares_shards, BN_CTX* ctx);

//生成门限签名聚合阶段的聚合分片y_i*l_i
void generate_threshold_shards(BIGNUM* shards, BIGNUM* shares, int tid);

//累加份额恢复私钥，用于keygen和sign的TTP
void recover_secret(BIGNUM* secret, BIGNUM** shards, int shards_len);

#endif