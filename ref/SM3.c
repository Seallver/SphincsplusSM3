#include <stdint.h>
#include <string.h>
#include "utils.h"
#include "SM3.h"

// SM3 常量定义
static const uint32_t IV[8] = {
    0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600,
    0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E
};

// SM3 状态结构（56字节 = 32哈希值 + 24内部状态）
typedef struct {
    uint32_t hash[8];    // 当前哈希值
    uint64_t total;      // 已处理字节数
    uint32_t w[68];      // 消息扩展缓冲区
    uint8_t buffer[64];  // 未处理数据缓冲区
    size_t buflen;       // 缓冲区当前长度
} SM3_STATE;

/*--------------------- 内部辅助函数 ---------------------*/
static inline uint32_t rotl(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

static inline uint32_t P0(uint32_t x) {
    return x ^ rotl(x, 9) ^ rotl(x, 17);
}

static inline uint32_t P1(uint32_t x) {
    return x ^ rotl(x, 15) ^ rotl(x, 23);
}

static inline uint32_t FF(uint32_t x, uint32_t y, uint32_t z, int j) {
    return (j < 16) ? (x ^ y ^ z) : ((x & y) | (x & z) | (y & z));
}

static inline uint32_t GG(uint32_t x, uint32_t y, uint32_t z, int j) {
    return (j < 16) ? (x ^ y ^ z) : ((x & y) | ((~x) & z));
}

/*--------------------- 公开接口函数 ---------------------*/
void sm3_inc_init(uint8_t *state) {
    SM3_STATE *ctx = (SM3_STATE*)state;
    memcpy(ctx->hash, IV, sizeof(IV));
    ctx->total = 0;
    ctx->buflen = 0;
}

void sm3_inc_blocks(uint8_t *state, const uint8_t *in, size_t blocks) {
    SM3_STATE *ctx = (SM3_STATE*)state;
    
    while (blocks--) {
        // 消息扩展
        for (int i = 0; i < 16; ++i) {
            ctx->w[i] = __builtin_bswap32(*(uint32_t*)(in + i*4));
        }
        for (int i = 16; i < 68; ++i) {
            ctx->w[i] = P1(ctx->w[i-16] ^ ctx->w[i-9] ^ rotl(ctx->w[i-3], 15))
                       ^ rotl(ctx->w[i-13], 7) ^ ctx->w[i-6];
        }

        // 压缩函数
        uint32_t A = ctx->hash[0];
        uint32_t B = ctx->hash[1];
        uint32_t C = ctx->hash[2];
        uint32_t D = ctx->hash[3];
        uint32_t E = ctx->hash[4];
        uint32_t F = ctx->hash[5];
        uint32_t G = ctx->hash[6];
        uint32_t H = ctx->hash[7];

        for (int j = 0; j < 64; ++j) {
            uint32_t SS1 = rotl((rotl(A,12) + E + rotl(0x79CC4519, j%32)), 7);
            uint32_t SS2 = SS1 ^ rotl(A,12);
            uint32_t TT1 = FF(A,B,C,j) + D + SS2 + (ctx->w[j] ^ ctx->w[j+4]);
            uint32_t TT2 = GG(E,F,G,j) + H + SS1 + ctx->w[j];
            
            D = C;
            C = rotl(B,9);
            B = A;
            A = TT1;
            H = G;
            G = rotl(F,19);
            F = E;
            E = P0(TT2);
        }

        ctx->hash[0] ^= A;
        ctx->hash[1] ^= B;
        ctx->hash[2] ^= C;
        ctx->hash[3] ^= D;
        ctx->hash[4] ^= E;
        ctx->hash[5] ^= F;
        ctx->hash[6] ^= G;
        ctx->hash[7] ^= H;

        in += 64;
        ctx->total += 64;
    }
}

void sm3_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen) {
    SM3_STATE *ctx = (SM3_STATE*)state;
    uint64_t total = ctx->total + inlen;
    uint8_t pad[64] = {0x80};
    
    // 处理剩余数据
    if (ctx->buflen + inlen < 64) {
        memcpy(ctx->buffer + ctx->buflen, in, inlen);
        ctx->buflen += inlen;
        return;
    }
    
    // 处理完整块
    size_t fill = 64 - ctx->buflen;
    
    memcpy(ctx->buffer + ctx->buflen, in, fill);
    
    sm3_inc_blocks(state, ctx->buffer, 1);
    
    in += fill;
    inlen -= fill;
    
    // 处理剩余块
    if (inlen >= 64) {
        size_t blocks = inlen / 64;
        sm3_inc_blocks(state, in, blocks);
        in += blocks * 64;
        inlen %= 64;
    }

    // 保存未处理数据
    memcpy(ctx->buffer, in, inlen);
    ctx->buflen = inlen;

    // 添加填充
    size_t padlen = (total % 64 < 56) ? (56 - total%64) : (120 - total%64);
    *(uint64_t*)(pad + padlen - 8) = __builtin_bswap64(total << 3);
    sm3_inc_blocks(state, pad, (padlen + 8)/64);

    // 输出哈希值
    for (int i = 0; i < 8; ++i) {
        *(uint32_t*)(out + i*4) = __builtin_bswap32(ctx->hash[i]);
    }
}

void sm3(uint8_t *out, const uint8_t *in, size_t inlen) {
    uint8_t state[sizeof(SM3_STATE)];
    sm3_inc_init(state);
    sm3_inc_finalize(out, state, in, inlen);
}

void mgf1_SM3(unsigned char *out, unsigned long outlen,
	const unsigned char *in, unsigned long inlen)
{
SPX_VLA(uint8_t, inbuf, inlen+4);
unsigned char outbuf[SPX_SM3_OUTPUT_BYTES];
unsigned long i;

memcpy(inbuf, in, inlen);

/* While we can fit in at least another full block of SM3 output.. */
for (i = 0; (i+1)*SPX_SM3_OUTPUT_BYTES <= outlen; i++) {
  u32_to_bytes(inbuf + inlen, i);
  sm3(out, inbuf, inlen + 4);
  out += SPX_SM3_OUTPUT_BYTES;
}
/* Until we cannot anymore, and we fill the remainder. */
if (outlen > i*SPX_SM3_OUTPUT_BYTES) {
  u32_to_bytes(inbuf + inlen, i);
  sm3(outbuf, inbuf, inlen + 4);
  memcpy(out, outbuf, outlen - i*SPX_SM3_OUTPUT_BYTES);
}
}

void seed_state(spx_ctx *ctx) {
    uint8_t block[SPX_SM3_BLOCK_BYTES];
    size_t i;


	for (i = 0; i < SPX_N; ++i) {
        block[i] = ctx->pub_seed[i];
    }
	for (i = SPX_N; i < SPX_SM3_BLOCK_BYTES; ++i) {
		block[i] = 0;
	}
	
	/* block has been properly initialized for both SHA-256 and SHA-512 */

	sm3_inc_init(ctx->state_seeded);

	sm3_inc_blocks(ctx->state_seeded, block, 1);
}