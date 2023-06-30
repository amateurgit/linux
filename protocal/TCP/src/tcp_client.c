#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <unistd.h>

#define PORT 8888

void process(int sockfd)
{
    ssize_t size = 0;
    char buffer[1024];

    for(;;)
    {
        memset(buffer, 0, sizeof(buffer));

        size = read(STDIN_FILENO, buffer, 1024);
        if (strstr(buffer, "exit"))
        {
            return;
        }
        else
        {
            write(sockfd, buffer, size);
        }

        size = read(sockfd, buffer, 1024);
        write(STDOUT_FILENO, buffer, size);
    }
}

int main(int argc, char *argv[])
{
    int sockfd = -1;
    struct sockaddr_in server_addr;
    int ret = -1;

    if (argc != 2)
    {
        printf("usage : ./tcp_client ip\n");
        exit(EXIT_FAILURE);
    }

    /* 1. 套接字  */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    /* 2. 连接 */
    ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* 3. 读写 */
    process(sockfd);

    /* 4. 结束 */
    close(sockfd);

    exit(EXIT_SUCCESS);
}
