#include <stdint.h>
#include <string.h>

#include "thash.h"
#include "address.h"
#include "params.h"
#include "utils.h"
#include "SM3.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
void thash(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const spx_ctx *ctx, uint32_t addr[8])
{
    unsigned char outbuf[SPX_SM3_OUTPUT_BYTES];
    SPX_VLA(uint8_t, bitmask, inblocks * SPX_N);
    SPX_VLA(uint8_t, buf, SPX_N + SPX_SM3_OUTPUT_BYTES + inblocks*SPX_N);
    uint8_t sm3_state[40];
    unsigned int i;

    memcpy(buf, ctx->pub_seed, SPX_N);
    memcpy(buf + SPX_N, addr, SPX_SM3_ADDR_BYTES);
    mgf1_SM3(bitmask, inblocks * SPX_N, buf, SPX_N + SPX_SM3_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    memcpy(sm3_state, ctx->state_seeded, 40 * sizeof(uint8_t));

    for (i = 0; i < inblocks * SPX_N; i++) {
        buf[SPX_N + SPX_SM3_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sm3_inc_finalize(outbuf, sm3_state, buf + SPX_N,
                        SPX_SM3_ADDR_BYTES + inblocks*SPX_N);
    memcpy(out, outbuf, SPX_N);
}