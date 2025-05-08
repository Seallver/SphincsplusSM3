#ifndef PARAMS_H
#define PARAMS_H

#define str(s) #s
#define xstr(s) str(s)


#include xstr(params/params-PARAMS.h)

/* TSS */
#define PLAYERS SPX_D //参与方数
#define THRESHOLD PLAYERS - 1 //门限数

#define SPX_BYTES (SPX_N + SPX_FORS_BYTES + SPX_D * SPX_WOTS_BYTES +\
    SPX_FULL_HEIGHT * SPX_N)

/* Resulting SPX sizes. */
#define SPX_WOTS_AVG ((SPX_D - 1)/(THRESHOLD))
#define SPX_WOTS_LAST (SPX_WOTS_AVG + (SPX_D - 1) % (THRESHOLD))


#endif