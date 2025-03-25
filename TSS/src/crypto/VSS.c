#include "VSS.h"


// 初始化VSS上下文，各参与方保持质数、原根、多项式次数一致，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx, BIGNUM* p, BIGNUM* g) {
    ctx->p = BN_new();
    ctx->g = BN_new();
    ctx->share = BN_new();
    ctx->comms = (BIGNUM**)malloc((THRESHOLD) * sizeof(BIGNUM*));
    for (int i = 0; i < THRESHOLD; i++) {
        ctx->comms[i] = BN_new();
        BN_zero(ctx->comms[i]);
    }

    //这些参数由可信第三方生成并分发
    BN_copy(ctx->p, p);
    BN_copy(ctx->g, g);
}

//初始化TTP_VSS上下文
void TTP_VSS_init(TTP_VSS_ctx* ctx, BIGNUM* P, BIGNUM* G) {
    ctx->p = BN_new();
    ctx->g = BN_new();
    BN_copy(ctx->p, P);
    BN_copy(ctx->g, G);

    ctx->secret = BN_new();
    ctx->degree = THRESHOLD - 1;
    ctx->shares = (BIGNUM**)malloc((PLAYERS+1) * sizeof(BIGNUM*)); //给所有参与者一个秘密份额
    for (int i = 0; i <= PLAYERS; i++) {
        ctx->shares[i] = BN_new();
        BN_zero(ctx->shares[i]);
    }
    ctx->comms = (BIGNUM**)malloc((ctx->degree + 1) * sizeof(BIGNUM*));
    for (int i = 0; i <= ctx->degree; i++) {
        ctx->comms[i] = BN_new();
    }
    ctx->coeffs = (BIGNUM**)malloc((ctx->degree + 1) * sizeof(BIGNUM*));
    for (int i = 0; i <= ctx->degree; i++) {
        ctx->coeffs[i] = BN_new();
    }

    if (!ctx->coeffs || !ctx->shares || !ctx->comms) {
        // 内存分配失败
        fprintf(stderr, "VSS_init: Memory allocation failed\n");
        exit(1);
    }

    BN_CTX *BNctx = BN_CTX_new();

    //生成秘密
    generate_secret(ctx->secret, ctx->p);

    //生成多项式系数
    generate_coefficients(ctx->coeffs, ctx->degree, ctx->secret, ctx->p, BNctx);

    //生成承诺
    generate_commitments(ctx->comms, ctx->coeffs, ctx->degree, ctx->p, ctx->g, BNctx);

    //生成秘密份额
    generate_shares(ctx->shares, ctx->coeffs, ctx->degree, ctx->p, BNctx);

    BN_CTX_free(BNctx);
}


void generate_secret(BIGNUM* secret, BIGNUM* p) {
    // 生成随机数
    if (!BN_rand_range(secret, p)) {
        fprintf(stderr, "Failed to generate random number\n");
        exit(1);
    }
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
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM* p = BN_new();
    BIGNUM *g = BN_new();
    
    // 生成安全素数
    BN_generate_prime_ex(p, MIN_PRIME_BITS, 1, NULL, NULL, NULL);
    
    // 寻找原根
    g = find_primitive_root(p, ctx);
    
    BN_copy(PRIME, p);
    BN_copy(GENERATOR, g);

    // 释放内存
    BN_free(p);
    BN_free(g);
    BN_CTX_free(ctx);
}




void generate_coefficients(BIGNUM** coeffs, int degree, const BIGNUM* secret, const BIGNUM* prime, BN_CTX* ctx) {
    for (int i = 1; i <= degree; i++) {
        BN_rand_range(coeffs[i], prime);
    }
    BN_copy(coeffs[0], secret);
}


void evaluate_poly(BIGNUM* result, BIGNUM** coeffs, int degree, const BIGNUM* x, const BIGNUM* prime, BN_CTX* ctx) {
    BIGNUM *term = BN_new();
    BIGNUM *x_pow = BN_new();
    BN_one(x_pow);  // x_pow = 1
    BIGNUM* q = BN_new();
    BIGNUM* tmp = BN_new();
    BN_one(tmp);
    BN_sub(q, prime, tmp);
    for (int i = 0; i <= degree; i++) {
        // term = coeffs[i] * x^i
        BN_mod_mul(term, coeffs[i], x_pow, q, ctx);
        //result += term
        BN_mod_add(result, result, term, q, ctx);

        //x_pow *= x
        BN_mod_mul(x_pow, x_pow, x, q, ctx);
        
    }

    BN_free(term);
    BN_free(x_pow);
    BN_free(q);
    BN_free(tmp);
}

void generate_commitments(BIGNUM** comms, BIGNUM** coeffs, int degree, const BIGNUM* p, const BIGNUM* g, BN_CTX* ctx) {
    for (int i = 0; i <= degree; i++) {
        BN_mod_exp(comms[i], g, coeffs[i], p, ctx);
    }
    
}

//生成秘密份额
void generate_shares(BIGNUM** shares, BIGNUM** coeffs, int degree, const BIGNUM* p, BN_CTX* ctx) {
    BIGNUM* index = BN_new();

    for (int i = 0; i <= PLAYERS; i++) {
        BN_set_word(index, (unsigned long)i);
        evaluate_poly(shares[i], coeffs, degree, index, p, ctx);
    }

    BN_free(index);
}


int verify_share(const BIGNUM* share, const BIGNUM* x, const BIGNUM** comms, int degree, const BIGNUM* p, const BIGNUM* g) {
    BN_CTX* ctx = BN_CTX_new();
    int ret = 0;
    
    BIGNUM *lhs = BN_new();
    BIGNUM *rhs = BN_new();
    BIGNUM *term = BN_new();
    BIGNUM *exponent = BN_new();
    
    // lhs = g^share mod p
    BN_mod_exp(lhs, g, share, p, ctx);
    
    BN_one(rhs);
    BIGNUM* J = BN_new();
    BIGNUM* q = BN_new();
    // q = p - 1
    BIGNUM* tmp = BN_new();
    BN_one(tmp);
    BN_sub(q, p, tmp);
    

    char* indexx = BN_bn2dec(x);
    int index = atoi(indexx);
        
    for (int j = 0; j <= degree; j++) {
        BN_set_word(J, (unsigned long)j);
        // exponent = x^j
        BN_mod_exp(exponent, x, J, q, ctx);  
        
        // term = comms[j]^exponent mod p
        BN_mod_exp(term, comms[j], exponent, p, ctx);
        
        // rhs = (rhs * term) mod p
        BN_mod_mul(rhs, rhs, term, p, ctx);
    }
    ret = BN_cmp(lhs, rhs) == 0;
    
    BN_free(lhs);
    BN_free(rhs);
    BN_free(J);
    BN_free(q);
    BN_free(tmp);
    BN_free(term);
    BN_free(exponent);
    BN_CTX_free(ctx);
    return ret;
}
