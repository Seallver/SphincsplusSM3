#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "randombytes.h"
#include "params.h"

// 质数相关参数
#define MIN_PRIME_BITS 4    // 最小质数位数
#define MR_TEST_ROUNDS 5    // Miller-Rabin测试轮数

#define PARTY_SEEDBYTES 8  // 参与方种子(秘密)长度
#define RANDOM_NUM_BYTES 1  //uint64_t的随机数最多8个字节

typedef struct
{
    uint64_t p; // 质数
    uint64_t g; // 原根
    uint64_t secret; // 秘密
    uint64_t* shares; // 分享
    uint64_t* comms; // 承诺
    uint64_t* coeffs; // 多项式系数
    int degree; // 多项式次数
} VSS_ctx;

// 初始化VSS上下文，各参与方保持质数、原根、多项式次数一致，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx, uint64_t p, uint64_t g);

// 快速幂模运算
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod);

// Miller-Rabin素性测试
bool is_prime(uint64_t n, int k);

// 生成候选质数
uint64_t generate_prime_candidate(int bits);

// 生成质数
uint64_t generate_prime(int bits);

// 分解质因数
void prime_factors(uint64_t n, uint64_t* factors, int* count);

// 查找原根
uint64_t find_primitive_root(uint64_t p);

// 初始化加密参数
void init_crypto_params(uint64_t *PRIME, uint64_t *GENERATOR);

// 生成多项式系数（模PRIME-1）
void generate_coefficients(uint64_t *coeffs, int degree, uint64_t secret, uint64_t PRIME) ;

// 多项式求值
uint64_t evaluate_poly(uint64_t *coeffs, int degree, uint64_t x, uint64_t PRIME);

// 生成承诺
void generate_commitments(uint64_t *coeffs, uint64_t *comms, int degree, uint64_t PRIME, uint64_t GENERATOR);

// 验证分享
bool verify_share(uint64_t share, uint64_t x, uint64_t* comms, int degree, uint64_t PRIME, uint64_t GENERATOR);