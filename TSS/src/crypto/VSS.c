#include "VSS.h"


// 初始化VSS上下文，各参与方保持质数、原根、多项式次数一致，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx, BIGNUM* p, BIGNUM* g) {
    //这些参数由可信第三方生成并分发
    ctx->p = p;
    ctx->g = g;
    ctx->degree = THRESHOLD - 1;

    //生成多项式系数
    ctx->coeffs = (BIGNUM**)malloc((ctx->degree + 1) * sizeof(BIGNUM*));
    generate_coefficients(ctx->coeffs, ctx->degree, ctx->secret, p);

    //生成承诺
    ctx->comms = (BIGNUM**)malloc((ctx->degree + 1) * sizeof(BIGNUM*));
    generate_commitments(ctx->coeffs, ctx->comms, ctx->degree, p, g);
}

// 寻找原根
BIGNUM* find_primitive_root(BIGNUM* p, BN_CTX* ctx) {
    BIGNUM* g = BN_new();
    BIGNUM* q = BN_new();
    BIGNUM* tmp = BN_new();
    BN_ULONG candidate = 2;

    // 计算 q = (p-1)/2
    BN_sub(q, p, BN_value_one());
    BN_rshift1(q, q);

    // 尝试候选数 2, 3, 5, ... 直到找到原根
    while (1) {
        BN_set_word(g, candidate);
        
        // 检查 g^2 mod p != 1
        BN_mod_sqr(tmp, g, p, ctx);
        if (BN_is_one(tmp)) {
            candidate++;
            continue;
        }

        // 检查 g^q mod p != 1
        BN_mod_exp(tmp, g, q, p, ctx);
        if (!BN_is_one(tmp)) {
            break;
        }
        candidate++;
    }

    BN_free(q);
    BN_free(tmp);
    return g;
}

void init_crypto_params(BIGNUM* PRIME, BIGNUM* GENERATOR) {
    BIGNUM *p = BN_new();
    BIGNUM *g = BN_new();
    
    // 生成安全素数
    BN_generate_prime_ex(p, MIN_PRIME_BITS, 0, NULL, NULL, NULL);

    printf("p = %s\n", BN_bn2dec(p));
    
    BN_CTX* ctx = BN_CTX_new();
    // 寻找原根
    g = find_primitive_root(p, ctx);

    printf("g = %s\n", BN_bn2dec(g));
    
    BN_copy(PRIME, p);
    BN_copy(GENERATOR, g);

    // 释放内存
    BN_free(p);
    BN_free(g);
    BN_CTX_free(ctx);
}


void generate_coefficients(BIGNUM** coeffs, int degree, const BIGNUM *secret, const BIGNUM *prime) {
    BN_CTX *ctx = BN_CTX_new();
    for (int i = 0; i <= degree; i++) {
        BN_rand_range(coeffs[i], prime);
    }
    BN_copy(coeffs[0], secret);
    BN_CTX_free(ctx);
}

void evaluate_poly(BIGNUM *result, BIGNUM** coeffs, int degree, const BIGNUM *x, const BIGNUM *prime) {
    BN_CTX *ctx = BN_CTX_new();
    BN_zero(result);
    
    BIGNUM *term = BN_new();
    BIGNUM *x_pow = BN_new();
    BIGNUM *mod_val = BN_new();
    BN_copy(x_pow, BN_value_one());
    
    for (int i = 0; i <= degree; i++) {
        // term = coeffs[i] * x^i
        BN_mul(term, coeffs[i], x_pow, ctx);
        BN_add(result, result, term);
        
        // x_pow *= x
        BN_mul(x_pow, x_pow, x, ctx);
    }
    
    // result mod (prime-1)
    BN_sub(mod_val, prime, BN_value_one());
    BN_mod(result, result, mod_val, ctx);
    
    BN_free(term);
    BN_free(x_pow);
    BN_free(mod_val);
    BN_CTX_free(ctx);
}

void generate_commitments(BIGNUM **comms, BIGNUM** coeffs, int degree, const BIGNUM *prime, const BIGNUM *g) {
    BN_CTX *ctx = BN_CTX_new();
    for (int i = 0; i <= degree; i++) {
        BN_mod_exp(comms[i], g, coeffs[i], prime, ctx);
    }
    BN_CTX_free(ctx);
}

int verify_share(const BIGNUM *share, const BIGNUM *x, const BIGNUM **comms, int degree, const BIGNUM *prime, const BIGNUM *g) {
    BN_CTX *ctx = BN_CTX_new();
    int ret = 0;
    
    BIGNUM *lhs = BN_new();
    BIGNUM *rhs = BN_new();
    BIGNUM *term = BN_new();
    BIGNUM *exponent = BN_new();
    
    // lhs = g^share mod p
    BN_mod_exp(lhs, g, share, prime, ctx);
    
    BN_one(rhs);
    for (int j = 0; j <= degree; j++) {
        // exponent = x^j
        BN_mod_exp(exponent, x, BN_new(), j, ctx);  // 需要实现快速幂
        
        // term = comms[j]^exponent mod p
        BN_mod_exp(term, comms[j], exponent, prime, ctx);
        
        // rhs = (rhs * term) mod p
        BN_mod_mul(rhs, rhs, term, prime, ctx);
    }
    
    ret = BN_cmp(lhs, rhs) == 0;
    
    BN_free(lhs);
    BN_free(rhs);
    BN_free(term);
    BN_free(exponent);
    BN_CTX_free(ctx);
    return ret;
}
