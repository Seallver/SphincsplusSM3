#include "keygen_handler.h"

int ttp_handler_recv(KeygenNet_ctx* ctx,int sock) {

    return 0;
}

int player_handler_recv(KeygenNet_ctx* ctx, int sock) {
    char buf[1024 + 1];  // 额外空间放终止符
    int bytes = recv(sock, buf, sizeof(buf) - 1, 0);  // 保留1字节给\0
    if (bytes <= 0) {
        perror("recv failed");
        return -1;
    }
    buf[bytes] = '\0';  // 终止字符串
    printf("Received: %s\n", buf);
    close(sock);
    return 0;
}

int ttp_handler_send(KeygenNet_ctx* ctx,int sock) {

    return 0;
}

int player_handler_send(KeygenNet_ctx* ctx, int sock) {
    const char *msg = "hello";
    int len = strlen(msg);
    int total_sent = 0;
    while (total_sent < len) {
        int sent = send(sock, msg + total_sent, len - total_sent, 0);
        if (sent <= 0) {
            perror("send failed");
            return -1;
        }
        total_sent += sent;
    }
    printf("Sent: %s\n", msg);
    close(sock);
    return 0;
}