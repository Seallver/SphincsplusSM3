#include "params.h"
#include "Json.h"
#include "net_settings.h"



BIGNUM* prime;
int threshold[SPX_D - 1];
pthread_barrier_t barrier;

void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }

    //初始化屏障
    pthread_barrier_init(&barrier, NULL, 0);

}

//打印签名内容
void print_sigma(const unsigned char* sigma) {
    printf("SIGMA:\n");
    printf("{\n");
    printf("\tR:\t");
    for (int i = 0; i < SPX_N; i++) {
        if (i >= 0 && i % 64 == 0) printf("\t");
        printf("%02x", sigma[i]);
    }
    printf("\n");
    sigma += SPX_N;

    printf("\tSig_FORS:\t");
    for (int i = 0; i < SPX_FORS_BYTES; i++) {
        if (i > 0 && i % 64 == 0) printf("\n\t\t\t");
        printf("%02x", sigma[i]);
    }
    printf("\n");
    sigma += SPX_FORS_BYTES;

    printf("\tSig_WOTS:\n");
    for (int i = 0; i < THRESHOLD + 1; i++) {
        printf("\t\t[%d] ", i);
        for (int j = 0; j < SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N; j++) {
            if (j > 0 && j % 64 == 0) printf("\n\t\t\t");
            if (j == 0)printf("\t");
            printf("%02x", sigma[j]);
        }
        printf("\n");
        sigma += SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;
    }
    printf("\n");
    printf("}\n");
    printf("SIGMA size: %.2f KB\n", SPX_BYTES / 1024.0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    init_params();

    unsigned char* sm = NULL;
    int smlen;
    int mlen;
    unsigned char pk[SPX_PK_BYTES];
    if (load_sm(pk, &sm, &smlen, &mlen, argv[1], SPX_BYTES)) {
        printf("Failed to load sm\n");
        return -1;
    }

    print_sigma(sm);

    printf("smlen = %d\n", SPX_BYTES);

    //验证签名
    if (tss_crypto_sign_verify(sm, SPX_BYTES, sm + SPX_BYTES, mlen, pk, THRESHOLD)) {
        printf("vrfy failed\n");
        return -1;
    }
    printf("vrfy success!\n");

    return 0;
}
