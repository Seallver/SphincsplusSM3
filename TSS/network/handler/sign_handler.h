#include "SSS.h"
#include "TSS_api.h"
#include "net_settings.h"
#include "net_context.h"

#define SAFE_FREE(p) do { free(p); (p) = NULL; } while (0)

#define BUFFER_SIZE 4096

static int send_bignum(int sock, const BIGNUM* num);

static int recv_bignum(int sock, BIGNUM* dest);

int player_conn_to_ttp(SignNet_ctx* ctx, int sock, int srv_id);

int ttp_listen_player(SignNet_ctx* ctx, int sock, int srv_id);

void aggregate_seed(SignNet_ctx * ctx);

int send_root(SignNet_ctx * ctx, int sock, int srv_id);

int recv_root(SignNet_ctx * ctx, int sock, int srv_id);

int ttp_recv_root(SignNet_ctx * ctx, int sock, int srv_id);

int listen_exchange_sig(SignNet_ctx * ctx, int sock, int srv_id);

int conn_exchange_sig(SignNet_ctx * ctx, int sock, int srv_id);

int bc_top_sig(SignNet_ctx * ctx, int sock, int srv_id);

int final_sig(SignNet_ctx * ctx, int sock, int srv_id);