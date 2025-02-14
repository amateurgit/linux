/*
   epoll 网络编程模型

       1. 创建 epool

           epoll_create1

       2. 初始化事件, 并关联 epool

           epoll_ctl

       3. 等待事件的发生

           epoll_wait

       4. 业务处理

           处理客户端连接

           处理业务
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

#include <fcntl.h>

#define PORT 8888
#define BACKLOG 5
#define MAX_EVENTS 10

int setnobloking(int fd)
{
    int flag;

    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0)
    {
        return -1;
    }

    flag |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flag) < 0)
    {
        return -1;
    }

    return 0;
}

/* 初始化套接口 */
static int __create_sockfd(void) {

    int sockfd = -1;
    struct sockaddr_in saddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if ( bind(sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0 ) {
        close(sockfd);
        return -1;
    }

    if ( listen(sockfd, BACKLOG) < 0 ) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int main(int argc, char *argv[]) {
    int listen_sock = -1, conn_sock;
    struct sockaddr_in caddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int nfds, epollfd;
    struct epoll_event ev, events[MAX_EVENTS];
    int n;

    int sockfd;
    ssize_t sz;
    char buff[1024];

    /* 初始化网络套接口 */
    listen_sock = __create_sockfd();
    if (listen_sock < 0) {
        printf("网络套接口初始化失败\n");
        exit(EXIT_FAILURE);
    }

    /* 1. 创建 epoll */
    epollfd = epoll_create1(0);
    if (-1 == epollfd) {
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    do {
        /* 2. 初始化 epoll 事件 */
        ev.events = EPOLLIN;
        ev.data.fd = listen_sock;
        if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev)) {
            break;
        }

        for (;;)
        {
            /* 3. 等待事件的发生 */
            nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
            if (-1 == nfds) {
                break;
            }

            /* 4. 事件处理 */
            for (n = 0; n < nfds; ++n)
            {
                sockfd = events[n].data.fd;
                if (sockfd == listen_sock)  /* 处理客户端连接  */
                {
                    conn_sock = accept(listen_sock, (struct sockaddr *)&caddr, &addrlen);
                    if (conn_sock < 0)
                        break;

                    if (setnobloking(conn_sock) < 0)
                        break;

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = conn_sock;
                    if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev))
                        break;
                }
                else  /* 业务处理 */
                {
                    sz = read(sockfd, buff, sizeof(buff));
                    if (sz > 0)
                    {
                        snprintf(buff, sizeof(buff), "%ld bytes altogether\n", sz - 1);
                        write(sockfd, buff, strlen(buff) + 1);
                    }
                    else if (0 == sz)
                    {
                        printf("对端 socket 已经关闭\n");
                    }
                    else
                    {
                        printf("read");
                    }
                }
            }
        }

    } while(0);

    close(epollfd);
    close(listen_sock);

    exit(EXIT_SUCCESS); 
}
