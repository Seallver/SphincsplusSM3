#include "communication.h"
#include "VSS.h"
#include "params.h"
#include "tContext.h"

void keygen_round_ttp(thread_ctx* ctx, BIGNUM* prime, BIGNUM* generator);

void keygen_round_player(thread_ctx* ctx, BIGNUM* prime, BIGNUM* generator);

