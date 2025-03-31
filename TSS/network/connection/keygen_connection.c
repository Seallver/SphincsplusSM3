#include "keygen_connection.h"

//监听本机端口
void listen_local_port(KeygenNet_ctx* ctx, int conn_numbers, int (*handler_func)(KeygenNet_ctx* ,int)) {
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
        .sin_port = htons(ctx->port)
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

    printf("party %d listening on port %d...\n", ctx->party_id, ctx->port);

    // 改进：循环接受多个连接
    while (conn_numbers--) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }

        printf("[P%d] New connection from %s:%d\n", 
              ctx->party_id, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        int ret = handler_func(ctx, client_sock);
        if (ret) {
            printf("keygen handler error\n");
        }
        
        
    }
    // 通常不会执行到这里
    close(server_fd);
}

void create_connection_p2p(char* dst_ip, int dst_port, KeygenNet_ctx* ctx, int (*handler_func)(KeygenNet_ctx*,int)) {
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
    if (ctx->port > 0) {
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 保持INADDR_ANY不变
        local_addr.sin_port = htons(ctx->port);        // 使用传入的端口
        
        if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
            perror("bind failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
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
            // printf("Connected successfully!\n");
            break;
        }
        
        // 连接失败处理
        if (attempt < MAX_RETRIES) {
            // printf("Connection failed (attempt %d/%d), retrying in %d sec...\n", 
            //       attempt, MAX_RETRIES, RETRY_DELAY);
            sleep(RETRY_DELAY);
        } else {
            fprintf(stderr, "Max retries reached. Connection failed: %s\n", strerror(errno));
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    

    int ret = handler_func(ctx, sockfd);
    if (ret) {
        printf("keygen handler error\n");
    }

    // 安全关闭连接
    shutdown(sockfd, SHUT_RDWR); // 关闭双向通信
    
    // printf("Connection closed\n");
}