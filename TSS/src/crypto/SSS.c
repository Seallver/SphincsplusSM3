#include "SSS.h"



void SSS_init(SSS_ctx* ctx) {
    BN_CTX *BNctx = BN_CTX_new();

    ctx->secret = BN_new();
    ctx->share = BN_new();
    ctx->coeffs = (BIGNUM**)malloc((DEGREE + 1) * sizeof(BIGNUM*));
    for (int i = 0; i <= DEGREE; i++) {
        ctx->coeffs[i] = BN_new();
    }
    
    if (!ctx->coeffs || !ctx->secret || !ctx->share) {
        // 内存分配失败
        fprintf(stderr, "SSS_init: Memory allocation failed\n");
        exit(1);
    }

    ctx->random_list = (BIGNUM**)malloc((PLAYERS + 1) *sizeof(BIGNUM*));
    for (int i = 0; i < PLAYERS + 1; i++) {
        ctx->random_list[i] = BN_new();
        BN_rand_range(ctx->random_list[i], prime);
    }

    //生成秘密多项式
    generate_secret(ctx->secret);
    generate_coefficients(ctx->coeffs, ctx->secret, BNctx);

    BN_CTX_free(BNctx);
}

void generate_secret(BIGNUM* secret) {
    // 生成随机数
    if (!BN_rand_range(secret, prime)) {
        fprintf(stderr, "Failed to generate random number\n");
        exit(1);
    }
}

void init_crypto_params(BIGNUM* PRIME) {
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM* p = BN_new();
    
    // 生成安全素数
    BN_generate_prime_ex(p, MIN_PRIME_BITS, 1, NULL, NULL, NULL);
    BN_copy(PRIME, p);

    // 释放内存
    BN_free(p);
    BN_CTX_free(ctx);
}

void generate_coefficients(BIGNUM** coeffs, const BIGNUM* secret, BN_CTX* ctx) {
    for (int i = 1; i <= DEGREE; i++) {
        BN_rand_range(coeffs[i], prime);
    }
    BN_copy(coeffs[0], secret);
}


void evaluate_poly(BIGNUM* result, BIGNUM** coeffs, const BIGNUM* x, BN_CTX* ctx) {
    BIGNUM *term = BN_new();
    BIGNUM *x_pow = BN_new();
    BN_one(x_pow);  // x_pow = 1

    for (int i = 0; i <= DEGREE; i++) {
        // term = coeffs[i] * x^i
        BN_mod_mul(term, coeffs[i], x_pow, prime, ctx);
        //result += term
        BN_mod_add(result, result, term, prime, ctx);

        //x_pow *= x
        BN_mod_mul(x_pow, x_pow, x, prime, ctx);
        
    }

    BN_free(term);
    BN_free(x_pow);
}

void generate_shares(BIGNUM** shares, BIGNUM** coeffs, BN_CTX* ctx) {
    BIGNUM* index = BN_new();

    for (int i = 1; i <= PLAYERS; i++) {
        BN_set_word(index, (unsigned long)i);
        evaluate_poly(shares[i], coeffs, index, ctx);
    }

    BN_free(index);
}

void aggregate_shares(BIGNUM* shares, BIGNUM** shares_shards, BN_CTX* ctx) {
    BIGNUM* tmp = BN_new();
    BN_zero(tmp);
    //累加f_i(j)得到y_j
    for (int i = 0; i < PLAYERS; i++) {
        BN_mod_add(tmp, tmp, shares_shards[i], prime, ctx);
    }
    BN_copy(shares, tmp);
    BN_free(tmp);
}

void generate_threshold_shards(BIGNUM* shards, BIGNUM* shares, int tid) {
    BN_CTX *ctx = BN_CTX_new();
    //获取x_k，初始化l_k
    BIGNUM* x_k = BN_new();
    BIGNUM* l_k = BN_new();
    BIGNUM* tmp = BN_new();
    BN_set_word(x_k, (unsigned long)tid);
    BN_one(l_k);

    //计算l_k
    for (size_t j = 0;j < THRESHOLD;j++) {
        if (threshold[j] == tid)continue;
        //获取x_j
        BIGNUM* x_j = BN_new();
        BN_set_word(x_j, (unsigned long)threshold[j]);
        
        //计算分母x_j-x_k的逆
        BN_sub(tmp, x_j, x_k);
        BN_mod_inverse(tmp, tmp, prime, ctx);

        //计算分式x_j/(x_j-x_k)
        BN_mod_mul(tmp, tmp, x_j, prime, ctx);

        //累乘到总分式上
        BN_mod_mul(l_k, l_k, tmp, prime, ctx);
    }
    //计算y_k*l_k
    BN_mod_mul(shards, shares, l_k, prime, ctx);

    BN_free(x_k);
    BN_free(l_k);
    BN_free(tmp);
    BN_CTX_free(ctx);
}



void recover_secret(BIGNUM* secret, BIGNUM** shares, int shards_len) {
    BN_CTX* BNctx = BN_CTX_new();
    BIGNUM* tmp = BN_new();
    BN_zero(tmp);
    //累加份额恢复私钥
    for (int i = 0; i < shards_len; i++) {
        BN_mod_add(tmp, tmp, shares[i], prime, BNctx);
    }
    BN_copy(secret, tmp);

    BN_CTX_free(BNctx);
    BN_free(tmp);
}