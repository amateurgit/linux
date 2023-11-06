#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_SERV 8888
#define BUFF_LEN 256

static void udpserv_echo(int sockfd, struct sockaddr *client)
{
    int n;
    char buff[BUFF_LEN];
    int len;

    while (1)
    {
        len = sizeof(*client);
        n = recvfrom(sockfd, buff, BUFF_LEN, 0, client, &len);
        printf("%s\n", buff);
        sendto(sockfd, buff, n, 0, client, len);
    }

    return;
}

int main(int argc, char *argv[])
{
    /* 套接字 */
    int sockfd = socket(AF_INET,  SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* 地址 */
    struct sockaddr_in addr_serv, addr_clie;
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_serv.sin_port = htons(PORT_SERV);

    /* 绑定 */
    if (bind(sockfd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_SUCCESS);
    }

    /* 数据 */
    udpserv_echo(sockfd, (struct sockaddr *)&addr_clie);

    close(sockfd);

    exit(EXIT_SUCCESS);
}
