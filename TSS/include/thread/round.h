#include "communication.h"
#include "TSS_api.h"
#include "globals.h"

void keygen_round_ttp(thread_ctx* ctx);

void keygen_round_player(thread_ctx* ctx);

void presign_round_player(thread_ctx* ctx);

void sign_round_ttp(thread_ctx* ctx);

void sign_round_player(thread_ctx* ctx);

int vrfy_round_player(thread_ctx *ctx);