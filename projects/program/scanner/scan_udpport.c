/* 
   UDP 端口扫描器

       逻辑
       ------------------------------------------------------------------------------------------------       
       |                                        正常 UDP 报文, 端口状态 OPEN                           |
       |                             响应    {                                                         |
       |                                        ICMP 报文(type=3,code=4), 目标不可达到, 端口状态 CLOSED|
       |                                                                                               |
       |发送 探测报文 到目标主机 {                                                                     |
       |                                        被目标主机防火墙过滤掉                                 |
       |                             无响应  {           或                                            |
       |                                        目标端口状态 OPEN, 未做响应                            |
       ------------------------------------------------------------------------------------------------       
    
       编译
           gcc -o scan scanner.c

 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

enum {
    OPEN = 0,
    CLOSED,
    OPEN_FILTERED,
    UNKNOWN,
};

struct udpport_stat
{
    int stat;
    char *desc;
} uportstat[] = {
    { OPEN, "Detected port is open" },
    { CLOSED, "Detected port is closed" },
    { OPEN_FILTERED, "Detected port is filtered by firewall or open, because cann't receive any msg from detected host" },
    { UNKNOWN, "Failed to detect" },
};

static int verify_ipaddr(char* ipstring)
{
    struct sockaddr_in6 addr6;
    struct sockaddr_in   addr4;
    int ret = 0;

    if(!ipstring || *ipstring =='\0')
    {
        return 0;
    }

    if( 1== inet_pton(AF_INET, ipstring, &addr4.sin_addr))
    {
        ret = 1;
    }
    else if (1== inet_pton(AF_INET6, ipstring, &addr6.sin6_addr))
    {
        ret = 1;
    }

    return ret;
}

static int detect_udpport(char *ipaddr, unsigned short dport)
{
    struct sockaddr_in dadddr;

    int sockfd = -1;
    int optval = 1;
    struct timeval tv = { 8, 0 };

    char sendbuff[1024] = { "UDP test" };
    char recvbuff[1024] = { 0 };
    int recv_len;


    bzero(&dadddr, sizeof(dadddr));
    dadddr.sin_family = AF_INET;
    dadddr.sin_addr.s_addr = inet_addr(ipaddr);
    dadddr.sin_port = htons(dport);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket < 0)
    {
        perror("socket");
        return -1;
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVERR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    if (sendto(sockfd, sendbuff, strlen(sendbuff), 0, (struct sockaddr *)&dadddr, sizeof(dadddr)) < 0)
    {
        perror("sendto");
        close(sockfd);
        return -1;
    }

    errno = 0;
    recv_len = recvfrom(sockfd, recvbuff, sizeof(recvbuff), 0, NULL, NULL);
    close(sockfd);

    if (-1 == recv_len)
    {
        if (EAGAIN == errno)
        {
            return OPEN_FILTERED;
        }
        else if (ECONNREFUSED == errno)
        {
            return CLOSED;
        }
        else
        {
            return UNKNOWN;
        }
    }

    return OPEN;
}

int main(int argc, char *argv[])
{
    char ipaddr[16];
    int dport = 0;

    int ret = -1;
    
    if (argc != 3)
    {
        printf("usage : ./scan <IPAddress> <Port>\n");
        exit(EXIT_FAILURE);
    }

    snprintf(ipaddr, sizeof(ipaddr), "%s", argv[1]);
    if (1 != verify_ipaddr(ipaddr))
    {
        printf("请输入合法的 IP 地址\n");
        exit(EXIT_FAILURE);
    }

    dport = atoi(argv[2]);
    if (dport < 0 || dport > 65535)
    {
        printf("请输入合法的端口<0-65535>\n");
        exit(EXIT_FAILURE);
    }

    ret = detect_udpport(ipaddr, dport);
    if (ret < 0)
    {
        printf("端口探测失败\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", uportstat[ret].desc);

    exit(EXIT_SUCCESS);
}
