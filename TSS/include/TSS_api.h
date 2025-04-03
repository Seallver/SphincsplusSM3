#ifndef SPX_API_H
#define SPX_API_H

#include <stddef.h>
#include <stdint.h>

#include "params.h"
#include "globals.h"

#define CRYPTO_ALGNAME "SPHINCS+"

#define CRYPTO_SECRETKEYBYTES SPX_SK_BYTES
#define CRYPTO_PUBLICKEYBYTES SPX_PK_BYTES
#define CRYPTO_BYTES SPX_BYTES
#define CRYPTO_SEEDBYTES 3*SPX_N

/*
 * Returns the length of a secret key, in bytes
 */
unsigned long long tss_crypto_sign_secretkeybytes(void);

/*
 * Returns the length of a public key, in bytes
 */
unsigned long long tss_crypto_sign_publickeybytes(void);

/*
 * Returns the length of a signature, in bytes
 */
unsigned long long tss_crypto_sign_bytes(void);

/*
 * Returns the length of the seed required to generate a key pair, in bytes
 */
unsigned long long tss_crypto_sign_seedbytes(void);

/*
 * Generates a SPHINCS+ key pair given a seed.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED]
 */
int tss_crypto_sign_seed_keypair(unsigned char *pk, unsigned char *sk,
                             const unsigned char *seed);

/*
 * Generates a SPHINCS+ key pair.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED]
 */
int tss_crypto_sign_keypair(unsigned char *pk, unsigned char *sk, const unsigned char *seed);


/**
 * Verifies a detached signature and message under a given public key.
 */
int tss_crypto_sign_verify(const uint8_t *sig, size_t siglen,
                       const uint8_t *m, size_t mlen, const uint8_t *pk, int t);


//用于预签名阶段TTP生成R
int tss_gen_R(unsigned char* R, const uint8_t* m, unsigned long long mlen,const uint8_t* sk);

//随机生成FORS的签名私钥
int tss_gen_FORS_seed(unsigned char* seed);

//预签名阶段参与方生成FORS签名，并得到addr
int tss_sign_FORS(unsigned char* sk, unsigned char* pk, uint32_t* wots_addr,
    unsigned char* mhash, unsigned char* root, unsigned char* m, unsigned char* sm,
    unsigned long long* smlen, unsigned long long mlen,
    uint64_t* tree, uint32_t* idx_leaf);

//签名阶段参与方生成WOTS签名
int tss_sign_WOTS(unsigned char* sk, unsigned char* pk, unsigned char* sig, unsigned char* root,
    uint32_t *wots_addr, uint32_t *tree_addr, uint32_t idx_leaf);

//最后一层WOTS签名交由TTP来完成
int tss_sign_WOTS_ttp(unsigned char* pk, unsigned char* root, uint32_t* wots_addr, uint32_t* tree_addr,
    uint32_t idx_leaf, unsigned char* sig, const unsigned char* sk);

//生成addr
int tss_gen_addr(int id, uint64_t* tree, uint32_t* idx_leaf, uint32_t* wots_addr, uint32_t* tree_addr);

//生成TTP的addr
int tss_gen_ttp_addr(unsigned char* pk, unsigned char* mhash, unsigned char* m, unsigned long long mlen,
    uint64_t* tree, uint32_t* idx_leaf, uint32_t* wots_addr, uint32_t* tree_addr, const unsigned char* R);

#endif
