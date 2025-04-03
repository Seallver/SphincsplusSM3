//声明一些多个文件用到的全局变量，由main函数初始化
#ifndef GLOBALS_H
#define GLOBALS_H
#include <openssl/bn.h>
#include <pthread.h>
#include "params.h"

//全局变量p，代表所有参与方均已知p
extern BIGNUM* prime;

//创建进程屏障
extern pthread_barrier_t barrier;

//乱序后前t个为门限参与方
extern int threshold[SPX_D - 1];

#endif