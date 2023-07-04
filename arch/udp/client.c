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
    struct sockaddr_in saddr;
    int addrlen = sizeof(struct sockaddr_in);

    char sendbuf[BUFSZ] = "Hello";
    char recvbuf[BUFSZ] = { 0 };

    int recvbytes, sendbytes;

    /* 1. 套接字 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* 2. 收发 */
    memset(&saddr, 0, addrlen);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    /* 向服务器发送数据 */
    sendbytes = sendto(sockfd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&saddr, addrlen);
    if (sendbytes < 0)
    {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* 从服务器接收数据 */
    recvbytes = recvfrom(sockfd, recvbuf, BUFSZ, 0, NULL, NULL);
    if (recvbytes < 0)
    {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", recvbuf);

    close(sockfd);
    exit(EXIT_SUCCESS);
}
