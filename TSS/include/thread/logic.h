#include "round.h"
#include "globals.h"


//keygen参与方线程逻辑
int keygen_player_logic(thread_ctx* ctx);

//keygen可信第三方线程逻辑
int keygen_TTP_logic(thread_ctx* ctx);

//sign门限参与方线程逻辑
int sign_player_logic(thread_ctx* ctx);

//sign可信第三方线程逻辑
int sign_TTP_logic(thread_ctx* ctx);

//verify参与方线程逻辑
int vrfy_player_logic(thread_ctx* ctx);