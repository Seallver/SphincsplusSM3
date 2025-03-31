#include "SSS.h"
#include "TSS_api.h"
#include "net_settings.h"
#include "net_context.h"

int ttp_handler_recv(KeygenNet_ctx* ctx, int sock);

int player_handler_recv(KeygenNet_ctx* ctx, int sock);

int ttp_handler_send(KeygenNet_ctx* ctx, int sock);

int player_handler_send(KeygenNet_ctx* ctx, int sock);