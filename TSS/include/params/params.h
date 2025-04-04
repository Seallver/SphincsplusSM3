#ifndef PARAMS_H
#define PARAMS_H

#define str(s) #s
#define xstr(s) str(s)
#define IS_PRINT 1 //是否打印线程状态信息,签名内容等


#include xstr(params/params-PARAMS.h)


/* TSS */
#define PLAYERS 3 //参与方数
#define THRESHOLD PLAYERS - 1 //门限数

#ifdef TSS
    /* Resulting SPX sizes. */
    #define SPX_BYTES (SPX_N + SPX_FORS_BYTES + (THRESHOLD + 1) * SPX_WOTS_BYTES +\
        (THRESHOLD + 1)* SPX_TREE_HEIGHT * SPX_N)
#else
    /* Resulting SPX sizes. */
    #define SPX_BYTES (SPX_N + SPX_FORS_BYTES + SPX_D * SPX_WOTS_BYTES +\
        SPX_FULL_HEIGHT * SPX_N)
#endif

#endif