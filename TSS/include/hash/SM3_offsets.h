#ifndef SM3_OFFSETS_H_
#define SM3_OFFSETS_H_

/*
 * Offsets of various fields in the address structure when we use SM3 as
 * the Sphincs+ hash function
 */

#define SPX_OFFSET_LAYER     0   /* The byte used to specify the Merkle tree layer */
#define SPX_OFFSET_TREE      1   /* The start of the 8 byte field used to specify the tree */
#define SPX_OFFSET_TYPE      9   /* The byte used to specify the hash type (reason) */
#define SPX_OFFSET_KP_ADDR   10  /* The start of the 4 byte field used to specify the key pair address */ 
#define SPX_OFFSET_CHAIN_ADDR 17  /* The byte used to specify the chain address (which Winternitz chain) */
#define SPX_OFFSET_HASH_ADDR 21  /* The byte used to specify the hash address (where in the Winternitz chain) */
#define SPX_OFFSET_TREE_HGT  17  /* The byte used to specify the height of this node in the FORS or Merkle tree */
#define SPX_OFFSET_TREE_INDEX 18 /* The start of the 4 byte field used to specify the node in the FORS or Merkle tree */

#define SPX_SM3 1

#endif /* SM3_OFFSETS_H_ */