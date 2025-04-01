#include "SSS.h"
#include "TSS_api.h"
#include "net_settings.h"
#include "net_context.h"

#define SAFE_FREE(p) do { free(p); (p) = NULL; } while (0)

#define BUFFER_SIZE 4096

static int send_bignum(int sock, const BIGNUM* num);

static int recv_bignum(int sock, BIGNUM* dest);

int ttp_handler_recv(KeygenNet_ctx* ctx, int sock, int srv_id);

int player_handler_recv(KeygenNet_ctx* ctx, int sock, int srv_id);

int player_handler_send(KeygenNet_ctx* ctx, int sock, int srv_id);

int player_handler_send_ttp(KeygenNet_ctx* ctx, int sock, int srv_id);

void complete_pk(KeygenNet_ctx* ctx);