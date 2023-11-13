#include <stdio.h>
#include <stdlib.h>    /* exit */

#include <arpa/inet.h>

static int check_ipstring(char* ipstring)
{
    struct sockaddr_in6 addr6;
    struct sockaddr_in   addr4;
    int ret = 0;

    if(!ipstring || *ipstring =='\0')
    {
        return 0;
    }

    if( 1 == inet_pton(AF_INET, ipstring, &addr4.sin_addr))
    {
        ret = 1;
    }
    else if (1 == inet_pton(AF_INET6, ipstring, &addr6.sin6_addr))
    {
        ret = 1;
    }

    return ret;
}


int main(int argc, char *argv[])
{
    char *ipaddr = "259.168.1.1";

    printf("%s 是 :", ipaddr);
    if (check_ipstring(ipaddr))
    {
        printf(" 合法地址\n");
    }
    else
    {
        printf(" 非法地址\n");
    }

    exit(EXIT_SUCCESS);
}
