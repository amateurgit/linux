#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <unistd.h>

#define PORT 8888
#define BACKLOG 2

/* 服务器对客户端的处理 */
void process(int client_sock)
{
    ssize_t size = 0;
    char buffer[1024];

    for(;;)
    {
        memset(buffer, 0, sizeof(buffer));
        size = read(client_sock, buffer, 1024);

        snprintf(buffer, sizeof(buffer), "%ld bytes altogether\n", size);
        write(client_sock, buffer, strlen(buffer) + 1);
    }
}


int main(int argc, char *argv[])
{
    int listen_sock = -1;
    int client_sock = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    int ret = -1;

    /* 1. 套接字 */
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    /* 2. 绑定 */
    ret = bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("bind");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    /* 3. 设置侦听队列长度 ( 已完成三次握手的 挂起的 连接队列 ) */
    ret = listen(listen_sock, BACKLOG);
    if (ret < 0)
    {
        perror("listen");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        /* 4. 接收连接 ( 三次握手之后, 从侦听队列中获取客户端连接进行处理 ) */
        client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addrlen);

        int pid = fork();
        if (0 == pid) /* 子进程 */
        {
            close(listen_sock);

            /* 5. 读写 */
            process(client_sock);
        }
        else /* 父进程 */
        {
            close(client_sock);
        }

    }

    /* 6. 释放资源 */
    close(listen_sock);

    exit(EXIT_SUCCESS);
}
