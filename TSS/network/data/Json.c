#include "Json.h"

// 将BIGNUM转换为十六进制字符串（用于JSON存储）
static char* bn_to_hex(const BIGNUM* bn) {
    if (!bn) return NULL;
    char* hex = BN_bn2hex(bn);
    char* result = strdup(hex);
    OPENSSL_free(hex);
    return result;
}

static char* base64_encode(const unsigned char* data, size_t len) {
    BIO *bio, *b64;
    BUF_MEM *buf_mem;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 禁止添加换行符
    BIO_write(bio, data, len);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buf_mem);

    char* result = malloc(buf_mem->length + 1);
    memcpy(result, buf_mem->data, buf_mem->length);
    result[buf_mem->length] = '\0';

    BIO_free_all(bio);
    return result;
}

// 将Base64字符串解码为二进制数据
static unsigned char* base64_decode(const char* input, size_t* out_len) {
    BIO *bio, *b64;
    size_t len = strlen(input);

    unsigned char* buffer = malloc(len);
    if (!buffer) return NULL;

    bio = BIO_new_mem_buf(input, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    *out_len = BIO_read(bio, buffer, len);
    BIO_free_all(bio);

    return buffer;
}

// 序列化KeygenNet_ctx结构体
cJSON* keygen_ctx_to_json(const KeygenNet_ctx* ctx) {
    cJSON* json = cJSON_CreateObject();
    if (!json) return NULL;

    // 序列化网络信息
    cJSON_AddNumberToObject(json, "party_id", ctx->party_id);
    cJSON_AddStringToObject(json, "local_ip", ctx->local_ip);
    cJSON_AddNumberToObject(json, "port", ctx->port);

    // 序列化公钥（Base64编码）
    char* pk_base64 = base64_encode(ctx->pk, SPX_PK_BYTES);
    cJSON_AddStringToObject(json, "pk", pk_base64);
    free(pk_base64);

    char* hex = bn_to_hex(ctx->sss_ctx->share);
    cJSON_AddStringToObject(json, "share", hex);

    hex = bn_to_hex(ctx->sss_ctx->secret);
    cJSON_AddStringToObject(json, "secret", hex);

    free(hex);

    return json;
}

// 写入JSON文件
int save_ctx_to_file(const KeygenNet_ctx* ctx, const char* filename) {
    cJSON* json = keygen_ctx_to_json(ctx);
    if (!json) return -1;

    char* json_str = cJSON_Print(json);
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    fputs(json_str, fp);
    fclose(fp);

    cJSON_Delete(json);
    free(json_str);
    return 0;
}

// 从十六进制字符串创建BIGNUM
static BIGNUM* hex_to_bn(const char* hex) {
    if (!hex) return NULL;
    BIGNUM* bn = BN_new();
    BN_hex2bn(&bn, hex);
    return bn;
}

// 从JSON解析SignNet_ctx
int json_to_sign_ctx(SignNet_ctx* ctx, const char* json_str) {
    cJSON* json = cJSON_Parse(json_str);
    if (!json) return NULL;
 
    if (!ctx) {
        cJSON_Delete(json);
        return NULL;
    }

    // 解析基础字段
    cJSON* item = cJSON_GetObjectItem(json, "party_id");
    if (item) ctx->party_id = item->valueint;

    item = cJSON_GetObjectItem(json, "local_ip");
    if (item) ctx->local_ip = strdup(item->valuestring);

    item = cJSON_GetObjectItem(json, "port");
    if (item) ctx->port = item->valueint;

    // 解析公钥（Base64）
    item = cJSON_GetObjectItem(json, "pk");
    if (item && cJSON_IsString(item)) {
        size_t decoded_len;
        unsigned char* decoded = base64_decode(item->valuestring, &decoded_len);
        
        if (decoded && decoded_len == SPX_PK_BYTES) {
            memcpy(ctx->pk, decoded, SPX_PK_BYTES);
        } else {
            fprintf(stderr, "Invalid PK length or format\n");
        }
        free(decoded);
    }
    

    // 解析secret
    item = cJSON_GetObjectItem(json, "secret");
    if (item) ctx->secret = hex_to_bn(item->valuestring);

    // 解析share
    item = cJSON_GetObjectItem(json, "share");
    if (item) ctx->share = hex_to_bn(item->valuestring);

    cJSON_Delete(json);
    return 0;
}

// 从文件加载上下文
int load_ctx_from_file(SignNet_ctx* ctx, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* json_str = malloc(size + 1);
    fread(json_str, 1, size, fp);
    json_str[size] = '\0';
    fclose(fp);

    json_to_sign_ctx(ctx ,json_str);
    free(json_str);

    return 0;
}

