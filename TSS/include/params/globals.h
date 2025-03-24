#ifndef GLOBALS_H
#define GLOBALS_H
#include <openssl/bn.h>
#include <pthread.h>

//声明全局变量p和g，代表所有参与方均已知
extern BIGNUM* prime;
extern BIGNUM* generator;

//创建进程屏障
extern pthread_barrier_t barrier;

#endif