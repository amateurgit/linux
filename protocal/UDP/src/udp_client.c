#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_SERV 8888
#define BUFF_LEN 256

static void udpclie_echo(int sockfd, struct sockaddr *server)
{
    char buff[BUFF_LEN] = "UDP TEST";
    char  recv_buff[BUFF_LEN] = { 0 };
    int len = sizeof(*server);

    sendto(sockfd, buff, strlen(buff), 0, server, len);
    recvfrom(sockfd, recv_buff, BUFF_LEN, 0, NULL, NULL);
    printf("%s\n", recv_buff);

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
    struct sockaddr_in addr_serv = { 0 };
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_serv.sin_port = htons(PORT_SERV);

    udpclie_echo(sockfd, (struct sockaddr *)&addr_serv);

    close(sockfd);

    exit(EXIT_SUCCESS);
}
