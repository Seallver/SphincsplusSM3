#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include "../net_settings.h"
#include "../connection/keygen_connection.h"
#include "../handle/keygen_handle.h"

#define MAX_EVENTS 1024
#define BUFFER_SIZE 4096
#define PORT 8080

//从net_settings中初始化参数
void init_params() {
    prime = BN_new();
    BN_dec2bn(&prime, P);
    for (int i = 0; i < THRESHOLD; i++) {
        threshold[i] = T[i];
    }
}

// 设置socket非阻塞
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 处理客户端数据
void handle_client_data(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t n = recv(fd, buffer, BUFFER_SIZE, 0);
    
    if (n > 0) {
        // 处理收到的消息
        printf("Received %zd bytes from fd %d: %.*s\n", 
              n, fd, (int)n, buffer);
        
        // 简单回显
        send(fd, buffer, n, 0);
    } else if (n == 0) {
        // 客户端关闭连接
        printf("Client fd %d disconnected\n", fd);
        close(fd);
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("recv error");
            close(fd);
        }
    }
}

int main() {
    init_params();

    int server_fd, epoll_fd;
    struct sockaddr_in addr;
    struct epoll_event ev, events[MAX_EVENTS];
    
    // 创建TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置SO_REUSEADDR
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定地址
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    // 创建epoll实例
    if ((epoll_fd = epoll_create1(0)) < 0) {
        perror("epoll_create1 failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 添加server socket到epoll
    ev.events = EPOLLIN | EPOLLET;  // 边缘触发模式
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    // 事件循环
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 处理新连接
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int client_fd = accept(server_fd, 
                                     (struct sockaddr*)&client_addr,
                                     &addr_len);
                if (client_fd < 0) {
                    perror("accept failed");
                    continue;
                }

                printf("New connection from %s:%d\n",
                      inet_ntoa(client_addr.sin_addr),
                      ntohs(client_addr.sin_port));

                // 设置非阻塞并添加到epoll
                set_nonblocking(client_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                    perror("epoll_ctl client failed");
                    close(client_fd);
                }
            } else {
                // 处理客户端数据
                handle_client_data(events[i].data.fd);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}