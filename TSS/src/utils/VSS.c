#include "VSS.h"


// 初始化VSS上下文，各参与方保持质数、原根、多项式次数一致，独立生成秘密和多项式系数
void VSS_init(VSS_ctx* ctx, uint64_t p, uint64_t g) {
    //这些参数由可信第三方生成并分发
    ctx->p = p;
    ctx->g = g;
    ctx->degree = THRESHOLD - 1;

    //随机生成秘密
    unsigned char party_seed[PARTY_SEEDBYTES];
    randombytes(party_seed, PARTY_SEEDBYTES);

    uint64_t* seed = (uint64_t*)party_seed;
    ctx->secret = *seed;

    //生成多项式系数
    ctx->coeffs = (uint64_t*)malloc((ctx->degree + 1) * sizeof(uint64_t));
    generate_coefficients(ctx->coeffs, ctx->degree, ctx->secret, p);

    //生成承诺
    ctx->comms = (uint64_t*)malloc((ctx->degree + 1) * sizeof(uint64_t));
    generate_commitments(ctx->coeffs, ctx->comms, ctx->degree, p, g);
}


uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

bool is_prime(uint64_t n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    // 分解n-1为d*2^s
    uint64_t d = n - 1;
    while (d % 2 == 0) d /= 2;

    for (int i = 0; i < k; i++) {
        uint64_t a = 2 + rand() % (n - 4);
        uint64_t x = 1, m = a, e = d;
        
        // 计算a^d % n
        while (e > 0) {
            if (e % 2 == 1) x = (x * m) % n;
            m = (m * m) % n;
            e /= 2;
        }
        if (x == 1 || x == n - 1) continue;

        // 二次探测
        for (uint64_t r = 1; r < d; r *= 2) {
            x = (x * x) % n;
            if (x == n - 1) break;
            if (x == 1) return false;
        }
        if (x != n - 1) return false;
    }
    return true;
}

uint64_t generate_prime_candidate(int bits) {
    uint64_t p;
    do {
        p = rand();
        p |= (1ULL << (bits - 1)); // 确保最高位为1
        p |= 1;                    // 确保是奇数
    } while (p < (1ULL << (bits - 1)));
    return p;
}

uint64_t generate_prime(int bits) {
    uint64_t p;
    do {
        p = generate_prime_candidate(bits);
    } while (!is_prime(p, MR_TEST_ROUNDS)); // 5轮Miller-Rabin测试
    return p;
}

void prime_factors(uint64_t n, uint64_t* factors, int* count) {
    *count = 0;
    if (n % 2 == 0) factors[(*count)++] = 2;
    while (n % 2 == 0) n /= 2;

    for (uint64_t i = 3; i*i <= n; i += 2) {
        if (n % i == 0) {
            factors[(*count)++] = i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 2) factors[(*count)++] = n;
}

uint64_t find_primitive_root(uint64_t p) {
    if (!is_prime(p, 5)) return 0;

    uint64_t factors[100];
    int factor_count = 0;
    prime_factors(p - 1, factors, &factor_count);

    for (uint64_t g = 2; g < p; g++) {
        bool valid = true;
        for (int i = 0; i < factor_count; i++) {
            if (mod_exp(g, (p-1)/factors[i], p) == 1) {
                valid = false;
                break;
            }
        }
        if (valid) return g;
    }
    return 0;
}

void init_crypto_params(uint64_t* PRIME, uint64_t* GENERATOR) {
    srand(time(NULL));
    if (IS_PRINT)
        printf("TTP Initializing crypto parameters...\n");
    // 生成质数
    uint64_t prime = generate_prime(MIN_PRIME_BITS);
    printf("Generated prime: %llu\n", prime);
    *PRIME = prime;
    // 查找原根
    uint64_t g = find_primitive_root(prime);
    printf("Generator: %llu\n", g);
    *GENERATOR = g;

    // 验证参数合法性
    if (g == 0) {
        fprintf(stderr, "Invalid crypto parameters!\n");
        exit(1);
    }

    if (IS_PRINT)
        printf("TTP Crypto parameters initialized\n");
}


void generate_coefficients(uint64_t *coeffs, int degree, uint64_t secret, uint64_t PRIME) {
    coeffs[0] = secret % (PRIME - 1);
    for (int i = 1; i <= degree; i++) {
        coeffs[i] = rand() % (PRIME - 1);
    }
}


uint64_t evaluate_poly(uint64_t *coeffs, int degree, uint64_t x, uint64_t PRIME) {
    uint64_t result = 0;
    for (int i = 0; i <= degree; i++) {
        result = (result + coeffs[i] * mod_exp(x, i, PRIME-1)) % (PRIME-1);
    }
    return result;
}

void generate_commitments(uint64_t *coeffs, uint64_t *comms, int degree, uint64_t PRIME, uint64_t GENERATOR) {
    for (int i = 0; i <= degree; i++) {
        comms[i] = mod_exp(GENERATOR, coeffs[i], PRIME);
    }
}

bool verify_share(uint64_t share, uint64_t x, uint64_t *comms, int degree, uint64_t PRIME, uint64_t GENERATOR) {
    uint64_t lhs = mod_exp(GENERATOR, share, PRIME);
    uint64_t rhs = 1;
    
    for (int j = 0; j <= degree; j++) {
        uint64_t exponent = mod_exp(x, j, PRIME - 1);
        uint64_t term = mod_exp(comms[j], exponent, PRIME);
        rhs = (rhs * term) % PRIME;
    }
    printf("lhs: %llu, rhs: %llu\n", lhs, rhs);
    return lhs == rhs;
}
