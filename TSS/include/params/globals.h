#ifndef GLOBALS_H
#define GLOBALS_H
#include <openssl/bn.h>
#include <pthread.h>
#include "params.h"

//声明全局变量p，代表所有参与方均已知
extern BIGNUM* prime;

//创建进程屏障
extern pthread_barrier_t barrier;

//乱序后前t个为门限参与方
extern int threshold[PLAYERS];

#endif