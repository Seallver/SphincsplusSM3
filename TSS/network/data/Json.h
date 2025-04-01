#ifndef CTXJSON
#define CTXJSON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include "cJSON.h"
#include "SSS.h"
#include "net_context.h"

static char* bn_to_hex(const BIGNUM* bn);
cJSON* keygen_ctx_to_json(const KeygenNet_ctx* ctx);
int save_ctx_to_file(const KeygenNet_ctx* ctx, const char* filename);

static BIGNUM* hex_to_bn(const char* hex);
int json_to_keygen_ctx(SignNet_ctx* ctx, const char* json_str);
int load_ctx_from_file(SignNet_ctx* ctx, const char* filename);



#endif