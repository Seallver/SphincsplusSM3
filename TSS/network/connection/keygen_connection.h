#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../handle/keygen_handle.h"

#define BUFFER_SIZE 4096
#define MAX_CLIENTS PLAYERS

#define MAX_RETRIES 5 // 最大重试次数
#define RETRY_DELAY 3  // 每次重试间隔（秒）

typedef struct {
    int party_id;
    char* local_ip;
    int port;
}Conn;

//保持端口监听
void listen_local_port(Conn* conn);

//创建p2p连接
void create_connection_p2p(char* ip, int port, Conn* conn);