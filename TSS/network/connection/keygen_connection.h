#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "net_context.h"
#include "threadpool.h"

#define MAX_CLIENTS PLAYERS

#define MAX_RETRIES 5 // 最大重试次数
#define RETRY_DELAY 3  // 每次重试间隔（秒）

//保持端口监听
void listen_local_port(KeygenNet_ctx* ctx, int conn_numbers, int (*handler_func)(KeygenNet_ctx*, int, int));

//创建p2p连接
void create_connection_p2p(char* ip, int port, KeygenNet_ctx* ctx, int (*handler_func)(KeygenNet_ctx*, int, int));