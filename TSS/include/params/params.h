#ifndef PARAMS_H
#define PARAMS_H

#define str(s) #s
#define xstr(s) str(s)
#define IS_PRINT 1 //是否打印线程状态信息,签名内容等


#include xstr(params/params-PARAMS.h)


/* TSS */
#define PLAYERS SPX_D - 1 //参与方数
#define THRESHOLD PLAYERS - 1 //门限数

#endif