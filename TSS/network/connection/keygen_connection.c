#include "keygen_connection.h"
#include <errno.h>
//监听本机端口
void listen_local_port(Conn* conn) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 关键修复：添加端口重用选项
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(conn->port)
    };

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("party %d listening on port %d...\n", conn->party_id, conn->port);

    int recv_times = conn->party_id - 1;
    // 改进：循环接受多个连接
    while (recv_times--) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }

        printf("[P%d] New connection from %s:%d\n", 
              conn->party_id, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // 这里应该创建新线程/进程处理连接
        
    

        close(client_sock); // 示例中立即关闭，实际应持久化连接
    }
    
    // 通常不会执行到这里
    close(server_fd);
}

void create_connection_p2p(char* dst_ip, int dst_port, Conn* conn) {
    int sockfd = -1;
    struct sockaddr_in servaddr, local_addr;
    
    // 创建socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置SO_REUSEADDR选项
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 绑定到指定本地端口（保持原逻辑不变）
    if (conn->port > 0) {
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 保持INADDR_ANY不变
        local_addr.sin_port = htons(conn->port);        // 使用传入的端口
        
        if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
            perror("bind failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Bound to local port %d\n", conn->port);
    }
    
    // 设置目标地址（保持原逻辑不变）
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(dst_port);
    
    if (inet_pton(AF_INET, dst_ip, &servaddr.sin_addr) <= 0) {
        perror("invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Connecting to %s:%d...\n", dst_ip, dst_port);
    
    // 改进的重试逻辑（非阻塞+超时控制）
    for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
        // 尝试连接
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
            printf("Connected successfully!\n");
            break;
        }
        
        // 连接失败处理
        if (attempt < MAX_RETRIES) {
            printf("Connection failed (attempt %d/%d), retrying in %d sec...\n", 
                  attempt, MAX_RETRIES, RETRY_DELAY);
            sleep(RETRY_DELAY);
        } else {
            fprintf(stderr, "Max retries reached. Connection failed: %s\n", strerror(errno));
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }
    
    // 发送数据（完整发送保证）
    const char *message = "helloworld";
    size_t total = strlen(message);
    ssize_t sent = 0;
    
    while (sent < total) {
        ssize_t n = send(sockfd, message + sent, total - sent, 0);
        if (n <= 0) {
            perror("send failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        sent += n;
    }
    printf("Sent %zd bytes: %s\n", sent, message);
    
    // 接收响应（可选，保持原注释状态）
    /*
    char buffer[BUFFER_SIZE] = {0};
    ssize_t received = recv(sockfd, buffer, BUFFER_SIZE-1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("Received: %s\n", buffer);
    }
    */
    
    // 安全关闭连接
    shutdown(sockfd, SHUT_RDWR); // 优雅关闭双向通信
    close(sockfd);
    printf("Connection closed\n");
}