#include <stdint.h>
#include <string.h>
#include "utils.h"
#include "SM3.h"

// SM3 常量IV定义
static const uint8_t iv_256[32] = {
    0x73, 0x80, 0x16, 0x6F,         //A
    0x49, 0x14, 0xB2, 0xB9,         //B
    0x17, 0x24, 0x42, 0xD7,         //C
    0xDA, 0x8A, 0x06, 0x00,         //D
    0xA9, 0x6F, 0x30, 0xBC,         //E
    0x16, 0x31, 0x38, 0xAA,         //F
    0xE3, 0x8D, 0xEE, 0x4D,         //G
    0xB0, 0xFB, 0x0E, 0x4E          //H
};

static uint32_t load_bigendian_32(const uint8_t *x) {
    return (uint32_t)(x[3]) | (((uint32_t)(x[2])) << 8) |
           (((uint32_t)(x[1])) << 16) | (((uint32_t)(x[0])) << 24);
}

static uint64_t load_bigendian_64(const uint8_t* x) {
    return (uint64_t)(x[7]) | (((uint64_t)(x[6])) << 8) |
           (((uint64_t)(x[5])) << 16) | (((uint64_t)(x[4])) << 24) |
           (((uint64_t)(x[3])) << 32) | (((uint64_t)(x[2])) << 40) |
           (((uint64_t)(x[1])) << 48) | (((uint64_t)(x[0])) << 56);
}

static void store_bigendian_32(uint8_t *x, uint64_t u) {
    x[3] = (uint8_t) u;
    u >>= 8;
    x[2] = (uint8_t) u;
    u >>= 8;
    x[1] = (uint8_t) u;
    u >>= 8;
    x[0] = (uint8_t) u;
}

static void store_bigendian_64(uint8_t* x, uint64_t u) {
    x[7] = (uint8_t) u;
    u >>= 8;
    x[6] = (uint8_t) u;
    u >>= 8;
    x[5] = (uint8_t) u;
    u >>= 8;
    x[4] = (uint8_t) u;
    u >>= 8;
    x[3] = (uint8_t) u;
    u >>= 8;
    x[2] = (uint8_t) u;
    u >>= 8;
    x[1] = (uint8_t) u;
    u >>= 8;
    x[0] = (uint8_t) u;
}


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

static inline uint32_t T(int j) {
    return (j < 16) ? (0x79cc4519) : (0x7a879d8a);
}

//分块哈希
void crypto_hashblocks_sm3(uint8_t* statebytes,
                            const uint8_t* in, size_t inlen) {
    
    uint32_t state[8];
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f;
    uint32_t g;
    uint32_t h;
    uint32_t SS1;
    uint32_t SS2;
    uint32_t TT1;
    uint32_t TT2;

    a = load_bigendian_32(statebytes + 0);
    state[0] = a;
    b = load_bigendian_32(statebytes + 4);
    state[1] = b;
    c = load_bigendian_32(statebytes + 8);
    state[2] = c;
    d = load_bigendian_32(statebytes + 12);
    state[3] = d;
    e = load_bigendian_32(statebytes + 16);
    state[4] = e;
    f = load_bigendian_32(statebytes + 20);
    state[5] = f;
    g = load_bigendian_32(statebytes + 24);
    state[6] = g;
    h = load_bigendian_32(statebytes + 28);
    state[7] = h;

    uint32_t w[68];

    uint32_t W[64];

    while (inlen >= 64) {

        a = state[0];
        b = state[1];
        c = state[2];
        d = state[3];
        e = state[4];
        f = state[5];
        g = state[6];
        h = state[7];

        for (size_t i = 0;i < 16;i++) {
            w[i] = load_bigendian_32(in + i * 4);
        }
        for (size_t i = 16;i < 68;i++) {
            w[i] = P1(w[i - 16] ^ w[i - 9] ^ rotl(w[i - 3], 15)) ^ rotl(w[i - 13], 7) ^ w[i - 6];
        }
        for (size_t i = 0;i < 64;i++) {
            W[i] = w[i] ^ w[i + 4];
        }

        for (int j = 0;j < 64;j++) {
            SS1 = rotl((rotl(a, 12) + e + rotl(T(j), j)), 7);
            SS2 = SS1 ^ (rotl(a, 12));
            TT1 = FF(a, b, c, j) + d + SS2 + W[j];
            TT2 = GG(e, f, g, j) + h + SS1 + w[j];
            d = c;
            c = rotl(b, 9);
            b = a;
            a = TT1;
            h = g;
            g = rotl(f, 19);
            f = e;
            e = P0(TT2);
        }

        state[0] ^= a;
        state[1] ^= b;
        state[2] ^= c;
        state[3] ^= d;
        state[4] ^= e;
        state[5] ^= f;
        state[6] ^= g;
        state[7] ^= h;

        in += 64;
        inlen -= 64;
    }

    store_bigendian_32(statebytes + 0, state[0]);
    store_bigendian_32(statebytes + 4, state[1]);
    store_bigendian_32(statebytes + 8, state[2]);
    store_bigendian_32(statebytes + 12, state[3]);
    store_bigendian_32(statebytes + 16, state[4]);
    store_bigendian_32(statebytes + 20, state[5]);
    store_bigendian_32(statebytes + 24, state[6]);
    store_bigendian_32(statebytes + 28, state[7]);
}

void sm3_inc_init(uint8_t* state) {
    //state前32字节存哈希结果，后8字节存已处理的消息的长度
    for (size_t i = 0; i < 32; ++i) {
        state[i] = iv_256[i];
    }
    for (size_t i = 32; i < 40; ++i) {
        state[i] = 0;
    }
}

void sm3_inc_blocks(uint8_t* state, const uint8_t* in, size_t inblocks) {
    uint64_t bytes = load_bigendian_64(state + 32);
    
    crypto_hashblocks_sm3(state, in, 64 * inblocks);
    bytes += 64 * inblocks;

    store_bigendian_64(state + 32, bytes);
}


void sm3_inc_finalize(uint8_t* out, uint8_t* state, const uint8_t* in, size_t inlen) {
    uint8_t padded[128];
    uint64_t bytes = load_bigendian_64(state + 32) + inlen;

    //先对能满足512比特分组的数据做哈希
    crypto_hashblocks_sm3(state, in, inlen);
    //计算剩余长度和剩余数据起始地址
    in += inlen;
    inlen &= 63;
    in -= inlen;

    //复制剩余数据到填充区
    for (size_t i = 0; i < inlen; ++i) {
        padded[i] = in[i];
    }
    //填充开始时先填充一个1
    padded[inlen] = 0x80;

    if (inlen < 56) {
        // 如果剩余数据长度小于 56 字节，填充到 64 字节
        for (size_t i = inlen + 1; i < 56; ++i) {
            padded[i] = 0;
        }
        padded[56] = (uint8_t) (bytes >> 53);
        padded[57] = (uint8_t) (bytes >> 45);
        padded[58] = (uint8_t) (bytes >> 37);
        padded[59] = (uint8_t) (bytes >> 29);
        padded[60] = (uint8_t) (bytes >> 21);
        padded[61] = (uint8_t) (bytes >> 13);
        padded[62] = (uint8_t) (bytes >> 5);
        padded[63] = (uint8_t) (bytes << 3);
        crypto_hashblocks_sm3(state, padded, 64);
    }
    else {
        // 如果剩余数据长度大于等于 56 字节，填充到 128 字节
        for (size_t i = inlen + 1; i < 120; ++i) {
            padded[i] = 0;
        }
        padded[120] = (uint8_t) (bytes >> 53);
        padded[121] = (uint8_t) (bytes >> 45);
        padded[122] = (uint8_t) (bytes >> 37);
        padded[123] = (uint8_t) (bytes >> 29);
        padded[124] = (uint8_t) (bytes >> 21);
        padded[125] = (uint8_t) (bytes >> 13);
        padded[126] = (uint8_t) (bytes >> 5);
        padded[127] = (uint8_t) (bytes << 3);
        crypto_hashblocks_sm3(state, padded, 128);
    }

    for (size_t i = 0; i < 32; ++i) {
        out[i] = state[i];
    }
}

void sm3(uint8_t* out, const uint8_t* in, size_t inlen) {
    uint8_t state[40];

    sm3_inc_init(state);
    sm3_inc_finalize(out, state, in, inlen);
}




void mgf1_SM3(unsigned char* out, unsigned long outlen,
    const unsigned char* in, unsigned long inlen) {
    
    SPX_VLA(uint8_t, inbuf, inlen + 4);
    unsigned char outbuf[SPX_SM3_OUTPUT_BYTES];
    unsigned long i;

    memcpy(inbuf, in, inlen);

    for (i = 0; (i+1)*SPX_SM3_OUTPUT_BYTES <= outlen; i++) {
    u32_to_bytes(inbuf + inlen, i);
    sm3(out, inbuf, inlen + 4);
    out += SPX_SM3_OUTPUT_BYTES;
    }
    
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
	

	sm3_inc_init(ctx->state_seeded);

	sm3_inc_blocks(ctx->state_seeded, block, 1);
}