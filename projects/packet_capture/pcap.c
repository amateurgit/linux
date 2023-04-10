/*
   捕获数据包, 打印 mac 地址、上层协议类型、ip 地址
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>

int main(int argc, char *argv[])
{
    int sockfd, n;
    char buffer[1024];
    struct ethhdr *eth;
    struct iphdr *iph;
    struct in_addr addr;

    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sockfd < 0)
    {
	perror("socket");

	exit(-1);
    }

    while (1)
    {
	n = recv(sockfd, buffer, sizeof(buffer), 0);
	if (-1 == n)
	{
	    perror("recv");

	    exit(-1);
	}
	if (0 == n)
	{
	    continue;
	}

	eth = (struct ethhdr *)buffer;
	printf("Dest MAC addr : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
		eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	printf("Src  MAC addr : %02x:%02x:%02x:%02x:%02x:%02x\n", 
		eth->h_source[0], eth->h_source[1], eth->h_source[2],
		eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf("Proto : 0x%02x%02x\n", 
		eth->h_proto & 0x0f, eth->h_proto & 0xf0 >> 8);

	iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
	addr.s_addr = iph->saddr;
	printf("Source host:%s\n",inet_ntoa(addr));
	addr.s_addr = iph->daddr;
	printf("Dest host:%s\n",inet_ntoa(addr));

    }

    close(sockfd);

    exit(0);
}
