#ifndef SPX_SM3_H
#define SPX_SM3_H

#include "params.h"

#define SPX_SM3_BLOCK_BYTES 64  // SM3 的输入块大小为 64 字节
#define SPX_SM3_OUTPUT_BYTES 32 // SM3 的输出哈希值大小为 32 字节

#if SPX_SM3_OUTPUT_BYTES < SPX_N
    #error Linking against SM3 with N larger than 32 bytes is not supported
#endif

#define SPX_SM3_ADDR_BYTES 22

#include <stddef.h>
#include <stdint.h>

void sm3_inc_init(uint8_t *state);
void sm3_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void sm3_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void sm3(uint8_t* out, const uint8_t* in, size_t inlen);

#define mgf1_256 SPX_NAMESPACE(mgf1_256)
void mgf1_256(unsigned char *out, unsigned long outlen,
          const unsigned char *in, unsigned long inlen);

#define seed_state SPX_NAMESPACE(seed_state)
void seed_state(spx_ctx *ctx);

#endif