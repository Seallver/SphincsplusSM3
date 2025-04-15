#include "sign_round.h"



int sign_round_player(SignNet_ctx* ctx) {
    int index;
    for (int i = 0;i < ctx->t;i++) {
        if (ctx->party_id == threshold[i])
            index = i;
    }

    //初始化结构体参数
    int wots_levels = index == ctx->t - 1 ? SPX_WOTS_LAST : SPX_WOTS_AVG;
    ctx->sm = malloc(SPX_BYTES + ctx->mlen);
    ctx->sig_shard =
        malloc(wots_levels * (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N));
    ctx->smlen = 0;

    //计算门限份额
    BIGNUM* shares = BN_new();
    BN_copy(shares, ctx->share);
    generate_threshold_shards(ctx->share, shares, ctx->party_id, ctx->t);

    //发送共享份额给ttp，并等待接收R和FORS私钥种子
    sign_create_connection_p2p(ctx->ip_[0], ctx->port_[0], ctx,
                               player_conn_to_ttp);

    memset(ctx->wots_addr, 0, sizeof(ctx->wots_addr));
    memset(ctx->tree_addr, 0, sizeof(ctx->tree_addr));

 
    for (int i = 0;i < SPX_N;i++) {
        ctx->root[i] = 0;
    }

    tss_sign_FORS(ctx->sk, ctx->pk, ctx->wots_addr, ctx->mhash, ctx->root,
                  ctx->m, ctx->sm, &ctx->smlen, ctx->mlen, &ctx->tree,
                  &ctx->idx_leaf);
    

    ctx->sm += SPX_N + SPX_FORS_BYTES;

    //计算出自己WOTS签名的地址
    tss_gen_addr(index * SPX_WOTS_AVG, &ctx->tree, &ctx->idx_leaf,
                 ctx->wots_addr, ctx->tree_addr);
    
    unsigned char* sk = malloc(SPX_SK_BYTES);
    int len = BN_bn2bin(ctx->secret, sk);
    memcpy(ctx->sk, sk, SPX_N);

    if (index == 0) {
        //生成签名
        tss_sign_WOTS(ctx->sk, ctx->pk, ctx->sig_shard, ctx->root,
                      ctx->wots_addr, ctx->tree_addr, ctx->tree, ctx->idx_leaf,
                      wots_levels, index * SPX_WOTS_AVG);
        
        int sig_len = wots_levels * (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
        ctx->smlen += sig_len;
        memcpy(ctx->sm, ctx->sig_shard, sig_len);

        //向下一个门限层发送root
        sign_create_connection_p2p(ctx->ip_[threshold[index + 1]],
                                   ctx->port_[threshold[index + 1]], ctx,
                                   send_root);
        
    }
    else {
        //监听端口，收取root
        sign_listen_local_port(ctx, 1, recv_root);

        tss_sign_WOTS(ctx->sk, ctx->pk, ctx->sig_shard, ctx->root,
                      ctx->wots_addr, ctx->tree_addr,ctx->tree,
                      ctx->idx_leaf,wots_levels, index * SPX_WOTS_AVG);

        int sig_len = wots_levels * (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
        ctx->smlen += sig_len;
        memcpy(ctx->sm + (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index * SPX_WOTS_AVG,
               ctx->sig_shard, sig_len);
               

        // for (int i = 0; i < (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * SPX_WOTS_AVG; i++) {
        //   printf("%02x", (ctx->sm + (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * index * SPX_WOTS_AVG)[i]);
        // }
        // printf("\n");
        
        if (index == ctx->t - 1) {
          sign_create_connection_p2p(ctx->ip_[0], ctx->port_[0], ctx,
                                     send_root);
        }
        else {
          sign_create_connection_p2p(ctx->ip_[threshold[index + 1]],
                                     ctx->port_[threshold[index + 1]], ctx,
                                     send_root);
          
        }
    }

    //监听本机端口，等待连接，P2P发送签名份额
    int conn_numbers = index;
    if (conn_numbers)
      keygen_listen_local_port(ctx, conn_numbers, listen_exchange_sig);
    
    //主动与其他参与方建立连接
    for (int i = index + 1; i < ctx->t; i++) {
      keygen_create_connection_p2p(ctx->ip_[threshold[i]],
                                   ctx->port_[threshold[i]], ctx,
                                   conn_exchange_sig);
      
    }

    //主动与ttp连接，获取最终份额并聚合
    keygen_create_connection_p2p(ctx->ip_[0], ctx->port_[0], ctx, final_sig);
    ctx->sm += (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) * (SPX_D - 1);

    // for (int i = 0;i < (SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N) ;i++) {
    //     printf("%02x", ctx->sm[i]);
    // }
    // printf("\n");

    BN_free(shares);

    return 0;
}

int sign_round_ttp(SignNet_ctx* ctx) {
    ctx->tmp_shares = (BIGNUM**)malloc((ctx->n + 1) * sizeof(BIGNUM*));
    for(int i = 1; i <= ctx->n; i++) {
        ctx->tmp_shares[i] = BN_new();
        BN_zero(ctx->tmp_shares[i]);
    }

    //起一个线程等待计算总公私钥
    pthread_t thread;
    pthread_create(&thread, NULL, aggregate_seed, ctx);


    //监听本机端口，等待连接，此次连接需要收集共享份额并聚合，然后生成R和FORS私钥种子并发送回去
    int conn_numbers = ctx->t;
    sign_listen_local_port(ctx, conn_numbers, ttp_listen_player);

    //接收第二层root
    sign_listen_local_port(ctx, 1, ttp_recv_root);

    //生成顶层签名份额
    ctx->sig_shard = malloc(SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N);
    tss_sign_WOTS_ttp(ctx->pk, ctx->root, ctx->wots_addr,
        ctx->tree_addr, ctx->idx_leaf, ctx->sig_shard, ctx->sk);

    // for (int i = 0;i < SPX_WOTS_BYTES + SPX_TREE_HEIGHT * SPX_N;i++) {
    //     printf("%x", ctx->sig_shard[i]);
    // }
    // printf("\n");

    //将顶层签名发送给所有参与者
    conn_numbers = ctx->t;
    sign_listen_local_port(ctx, conn_numbers, bc_top_sig);

    return 0;
}