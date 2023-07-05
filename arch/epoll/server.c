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

int main(int argc, char *argv[])
{
    int listen_sock, conn_sock;
    struct sockaddr_in saddr, caddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int nfds, epollfd;
    struct epoll_event ev, events[MAX_EVENTS];
    int n;

    int sockfd;
    ssize_t sz;
    char buff[1024];

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    bzero(&saddr, addrlen);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if (bind(listen_sock, (struct sockaddr *)&saddr, addrlen) < 0)
    {
        perror("bind");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sock, BACKLOG) < 0)
    {
        perror("listen");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    epollfd = epoll_create1(0);
    if (-1 == epollfd)
    {
        perror("epoll_create1");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev))
    {
        perror("epoll_ctl : listen_sock");
        close(epollfd);
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (-1 == nfds)
        {
            perror("epoll_wait");
            close(epollfd);
            close(listen_sock);
            exit(EXIT_FAILURE);
        }

        for (n = 0; n < nfds; ++n)
        {
            sockfd = events[n].data.fd;
            if (sockfd == listen_sock)
            {
                conn_sock = accept(listen_sock, (struct sockaddr *)&caddr, &addrlen);
                if (conn_sock < 0)
                {
                    perror("accept");
                    close(epollfd);
                    close(listen_sock);
                    exit(EXIT_FAILURE);
                }

                if (setnobloking(conn_sock) < 0)
                {
                    perror("setnobloking");
                    close(epollfd);
                    close(listen_sock);
                    exit(EXIT_FAILURE);
                }

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev))
                {
                    perror("epoll_wait : conn_sock");
                    close(epollfd);
                    close(listen_sock);
                    exit(EXIT_FAILURE);
                }
            }
            else
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
                    perror("read");
                }
            }
        }
    }

    close(epollfd);
    close(listen_sock);

    exit(EXIT_SUCCESS); 
}
