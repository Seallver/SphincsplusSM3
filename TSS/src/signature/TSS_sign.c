#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "TSS_api.h"

#include "wots.h"
#include "fors.h"
#include "hash.h"
#include "thash.h"
#include "address.h"
#include "randombytes.h"
#include "utils.h"
#include "merkle.h"

/*
 * Returns the length of a secret key, in bytes
 */
unsigned long long tss_crypto_sign_secretkeybytes(void)
{
    return CRYPTO_SECRETKEYBYTES;
}

/*
 * Returns the length of a public key, in bytes
 */
unsigned long long tss_crypto_sign_publickeybytes(void)
{
    return CRYPTO_PUBLICKEYBYTES;
}

/*
 * Returns the length of a signature, in bytes
 */
unsigned long long tss_crypto_sign_bytes(void)
{
    return CRYPTO_BYTES;
}

/*
 * Returns the length of the seed required to generate a key pair, in bytes
 */
unsigned long long tss_crypto_sign_seedbytes(void)
{
    return CRYPTO_SEEDBYTES;
}

/*
 * Generates an SPX key pair given a seed of length
 * Format sk: [partial_SK_SEED || partial_SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int tss_crypto_sign_seed_keypair(unsigned char *pk, unsigned char *sk,
                             const unsigned char *seed)
{
    spx_ctx ctx;
    /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
    memcpy(sk, seed, CRYPTO_SEEDBYTES);

    memcpy(pk, sk + 2*SPX_N, SPX_N);

    memcpy(ctx.pub_seed, pk, SPX_N);
    memcpy(ctx.sk_seed, sk, SPX_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    initialize_hash_function(&ctx);

    /* Compute root node of the top-most subtree. */
    merkle_gen_root(sk + 3*SPX_N, &ctx);

    memcpy(pk + SPX_N, sk + 3*SPX_N, SPX_N);

    return 0;
}

/*
 * Generates an SPX key pair.
 * Format sk: [partial_SK_SEED || partial_SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int tss_crypto_sign_keypair(unsigned char *pk, unsigned char *sk, const unsigned char *seed)
{
    tss_crypto_sign_seed_keypair(pk, sk, seed);
    return 0;
}

int tss_sign_FORS(thread_ctx* thread_ctx)
{
    spx_ctx ctx;

    unsigned char optrand[SPX_N];

    memcpy(ctx.sk_seed, thread_ctx->sk, SPX_N);
    memcpy(ctx.pub_seed, thread_ctx->pk, SPX_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    initialize_hash_function(&ctx);

    set_type(thread_ctx->wots_addr, SPX_ADDR_TYPE_WOTS);

    /* Derive the message digest and leaf index from R, PK and M. */
    hash_message(thread_ctx->mhash, &thread_ctx->tree, &thread_ctx->idx_leaf, thread_ctx->sm, thread_ctx->pk, thread_ctx->m, thread_ctx->mlen, &ctx);
    thread_ctx->sm += SPX_N;
    set_tree_addr(thread_ctx->wots_addr, thread_ctx->tree);
    set_keypair_addr(thread_ctx->wots_addr, thread_ctx->idx_leaf);

    /* Sign the message hash using FORS. */
    fors_sign(thread_ctx->sm, thread_ctx->root, thread_ctx->mhash, &ctx, thread_ctx->wots_addr);

    thread_ctx->sm += SPX_FORS_BYTES;

    thread_ctx->smlen += SPX_FORS_BYTES + SPX_N;

    return 0;
}

int tss_gen_R(unsigned char* R, const uint8_t* m, const uint8_t* sk)
{
    size_t mlen = SPX_MLEN;
    spx_ctx ctx;
    const unsigned char *sk_prf = sk + SPX_N;
    const unsigned char *pk = sk + 2*SPX_N;

    unsigned char optrand[SPX_N];

    memcpy(ctx.sk_seed, sk, SPX_N);
    memcpy(ctx.pub_seed, pk, SPX_N);

    initialize_hash_function(&ctx);

    randombytes(optrand, SPX_N);

    gen_message_random(R, sk_prf, optrand, m, mlen, &ctx);

    return 0;
}

int tss_gen_FORS_seed(unsigned char* seed) {
    randombytes(seed, SPX_N);
}
int tss_gen_addr(thread_ctx* ctx) {
    for (int i = 0;i < ctx->tid;i++) {
        set_layer_addr(ctx->tree_addr, i);
        set_tree_addr(ctx->tree_addr, ctx->tree);

        copy_subtree_addr(ctx->wots_addr, ctx->tree_addr);
        set_keypair_addr(ctx->wots_addr, ctx->idx_leaf);

        if (i == ctx->tid - 1) break;
        ctx->idx_leaf = (ctx->tree & ((1 << SPX_TREE_HEIGHT) - 1));
        ctx->tree = ctx->tree >> SPX_TREE_HEIGHT;
    }
}

int tss_gen_ttp_addr(thread_ctx* thread_ctx, const unsigned char*R) {
    spx_ctx spxctx;
    set_type(thread_ctx->wots_addr, SPX_ADDR_TYPE_WOTS);
    set_type(thread_ctx->tree_addr, SPX_ADDR_TYPE_HASHTREE);
    memcpy(spxctx.pub_seed, thread_ctx->pk, SPX_N);
    hash_message(thread_ctx->mhash, &thread_ctx->tree, &thread_ctx->idx_leaf, R, thread_ctx->pk, thread_ctx->m, thread_ctx->mlen, &spxctx);
    set_tree_addr(thread_ctx->wots_addr, thread_ctx->tree);
    set_keypair_addr(thread_ctx->wots_addr, thread_ctx->idx_leaf);

    for (int i = 0;i < SPX_D;i++) {
        set_layer_addr(thread_ctx->tree_addr, i);
        set_tree_addr(thread_ctx->tree_addr, thread_ctx->tree);
        
        copy_subtree_addr(thread_ctx->wots_addr, thread_ctx->tree_addr);
        set_keypair_addr(thread_ctx->wots_addr, thread_ctx->idx_leaf);

        if (i == SPX_D - 1) break;
        thread_ctx->idx_leaf = (thread_ctx->tree & ((1 << SPX_TREE_HEIGHT) - 1));
        thread_ctx->tree = thread_ctx->tree >> SPX_TREE_HEIGHT;
    }
}


int tss_sign_WOTS(thread_ctx* thread_ctx,unsigned char * sig)
{
    spx_ctx ctx;
    memcpy(ctx.sk_seed, thread_ctx->sk, SPX_N);
    memcpy(ctx.pub_seed, thread_ctx->pk, SPX_N);

    initialize_hash_function(&ctx);

    merkle_sign(sig, thread_ctx->root, &ctx, thread_ctx->wots_addr, thread_ctx->tree_addr, thread_ctx->idx_leaf);
}

int tss_sign_WOTS_ttp(thread_ctx* thread_ctx, unsigned char * sig, const unsigned char* sk)
{
    spx_ctx ctx;
    memcpy(ctx.sk_seed, sk, SPX_N);
    memcpy(ctx.pub_seed, thread_ctx->pk, SPX_N);

    initialize_hash_function(&ctx);

    merkle_sign(sig, thread_ctx->root, &ctx, thread_ctx->wots_addr, thread_ctx->tree_addr, thread_ctx->idx_leaf);
}


/**
 * Verifies a detached signature and message under a given public key.
 */
int tss_crypto_sign_verify(const uint8_t *sig, size_t siglen,
                       const uint8_t *m, size_t mlen, const uint8_t *pk)
{
    spx_ctx ctx;
    const unsigned char *pub_root = pk + SPX_N;
    unsigned char mhash[SPX_FORS_MSG_BYTES];
    unsigned char wots_pk[SPX_WOTS_BYTES];
    unsigned char root[SPX_N];
    unsigned char leaf[SPX_N];
    unsigned int i;
    uint64_t tree;
    uint32_t idx_leaf;
    uint32_t wots_addr[8] = {0};
    uint32_t tree_addr[8] = {0};
    uint32_t wots_pk_addr[8] = {0};

    if (siglen != SPX_BYTES) {
        return -1;
    }

    memcpy(ctx.pub_seed, pk, SPX_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    initialize_hash_function(&ctx);

    set_type(wots_addr, SPX_ADDR_TYPE_WOTS);
    set_type(tree_addr, SPX_ADDR_TYPE_HASHTREE);
    set_type(wots_pk_addr, SPX_ADDR_TYPE_WOTSPK);

    /* Derive the message digest and leaf index from R || PK || M. */
    /* The additional SPX_N is a result of the hash domain separator. */
    hash_message(mhash, &tree, &idx_leaf, sig, pk, m, mlen, &ctx);
    sig += SPX_N;

    /* Layer correctly defaults to 0, so no need to set_layer_addr */
    set_tree_addr(wots_addr, tree);
    set_keypair_addr(wots_addr, idx_leaf);

    fors_pk_from_sig(root, sig, mhash, &ctx, wots_addr);
    sig += SPX_FORS_BYTES;
    
    int j = 0;
    /* For each subtree.. */
    for (i = 0; i < SPX_D; i++) {
        set_layer_addr(tree_addr, i);
        set_tree_addr(tree_addr, tree);

        copy_subtree_addr(wots_addr, tree_addr);
        set_keypair_addr(wots_addr, idx_leaf);

        copy_keypair_addr(wots_pk_addr, wots_addr);

        if (i == SPX_D - 1 || (i + 1 == threshold[j] && j < THRESHOLD)) {
            
            j++;
            /* The WOTS public key is only correct if the signature was correct. */
            /* Initially, root is the FORS pk, but on subsequent iterations it is
            the root of the subtree below the currently processed subtree. */
            wots_pk_from_sig(wots_pk, sig, root, &ctx, wots_addr);
            sig += SPX_WOTS_BYTES;

            /* Compute the leaf node using the WOTS public key. */
            thash(leaf, wots_pk, SPX_WOTS_LEN, &ctx, wots_pk_addr);

            /* Compute the root node of this subtree. */
            compute_root(root, leaf, idx_leaf, 0, sig, SPX_TREE_HEIGHT,
                &ctx, tree_addr);

            sig += SPX_TREE_HEIGHT * SPX_N;
        }
        /* Update the indices for the next layer. */
        idx_leaf = (tree & ((1 << SPX_TREE_HEIGHT) - 1));
        tree = tree >> SPX_TREE_HEIGHT;
    }


    /* Check if the root node equals the root node in the public key. */
    if (memcmp(root, pub_root, SPX_N)) {
        return -1;
    }

    return 0;
}
