#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSZ 65536
#define PORT 8888

int main(int argc, char *argv[])
{
    int sockfd = -1;
    struct sockaddr_in saddr, caddr;

    char buf[BUFSZ] = { 0 };
    int addrlen = sizeof(struct sockaddr_in);
    int recvbytes, sendbytes;

    /* 1. 套接字 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* 2. 绑定 */
    memset(&saddr, 0, addrlen);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&saddr, addrlen) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    /* 3. 收发数据 */
    while (1)
    {
        /* 接收数据, 并获取客户端地址信息 */
        recvbytes = recvfrom(sockfd, buf, BUFSZ, 0, (struct sockaddr *)&caddr, &addrlen);
        if (recvbytes < 0)
        {
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        /* 发送数据 */
        sendbytes = sendto(sockfd, buf, recvbytes, 0, (struct sockaddr *)&caddr, addrlen);
        if (sendbytes < 0)
        {
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);
    exit(EXIT_SUCCESS);
}
