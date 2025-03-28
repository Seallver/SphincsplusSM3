#define str(s) #s
#define xstr(s) str(s)
#define IS_PRINT 1 //是否打印线程状态信息,签名内容等


#include xstr(params/params-PARAMS.h)

/* TSS */
#define NUMBER_OF_THREADS SPX_D - 2 + 1 //线程数目，注意需要加上TTP
#define PLAYERS NUMBER_OF_THREADS - 1 //参与方数
#define THRESHOLD PLAYERS - 2 //门限数