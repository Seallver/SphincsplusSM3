#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../net_settings.h"
#include "../connection/keygen_connection.h"

void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }
}

int main(int argc, char* argv[]) {
    init_params();

    if (argc != 4) {
        printf("Usage: %s <party_id> <local_IP> <port> \n", argv[0]);
        return -1;
    }

    Conn* conn = (Conn*)malloc(sizeof(Conn));
    conn->party_id = atoi(argv[1]);
    conn->local_ip = malloc(strlen(argv[2]) + 1);
    memcpy(conn->local_ip, argv[2], strlen(argv[2]));
    conn->port = atoi(argv[3]);

    //监听本机端口，等待连接
    listen_local_port(conn);


    //主动与其他参与方建立连接
    for (int i = conn->party_id + 1; i <= PLAYERS; i++) {
        create_connection_p2p(IP[i], port[i], conn);
    }

    return 0;
}