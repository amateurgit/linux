/*
   原始套接字实现 ping 功能

   icmp_pack    封包

   icmp_unpack  解析封包

 */

#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>

#define K 1024
#define BUFFSIZE 72

static char send_buff[BUFFSIZE];
static char recv_buff[2 * K];

static char dest_str[80]; 
static int rawsock = 0;
static pid_t uid = 0;
static struct sockaddr_in dest;
static struct timeval tv_begin, tv_end, tv_interval;
static int alive = 0;
static short packet_send = 0;
static short packet_recv = 0;

typedef struct pingm_packet{
    struct timeval tv_begin;
    struct timeval tv_end;
    short seq;
    int flag;
}pingm_packet;

static pingm_packet pingpacket[128];

static void icmp_usage(void)
{
    printf("ping aaa.bbb.ccc.ddd\n");

    return;
}

static struct timeval icmp_tvsub(struct timeval end, struct timeval begin)
{
    struct timeval tv;

    tv.tv_sec = end.tv_sec - begin.tv_sec;
    tv.tv_usec = end.tv_usec - begin.tv_usec;

    if (tv.tv_usec < 0)
    {
	tv.tv_sec--;
	tv.tv_usec += 1000000;
    }

    return tv;
}

static void icmp_sigint(int signo)
{
    alive = 0;

    gettimeofday(&tv_end, NULL);
    tv_interval = icmp_tvsub(tv_end, tv_begin);

    return;
}

static pingm_packet *icmp_findpacket(int seq)
{
    int i = 0;
    pingm_packet *found = NULL;

    if (-1 == seq)
    {
	for (i = 0; i < 128; i++)
	{
	    if (0 == pingpacket[i].flag)
	    {
		found = &pingpacket[i];

		break;
	    }
	}
    }
    else if (seq > 0)
    {
	for (i = 0; i < 128; i++)
	{
	    if (seq == pingpacket[i].seq)
	    {
		found = &pingpacket[i];

		break;
	    }
	}
    }

    return found;

}

static unsigned short icmp_cksum(unsigned char *data,  int len)
{
    int sum=0;

    int odd = len & 0x01;

    while( len & 0xfffe)  {

	sum += *(unsigned short*)data;

	data += 2;

	len -=2;

    }

    if( odd) {

	unsigned short tmp = ((*data)<<8)&0xff00;

	sum += tmp;

    }

    sum = (sum >>16) + (sum & 0xffff);

    sum += (sum >>16);

    return ~sum;

}

/*

  0-----7-8-------15-16-----------------31
  类型   |  代码    |       校验和
  标识符            |       序列号
  数据
*/
/* ping 封包 */
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length)
{
    unsigned char i = 0;
    icmph->icmp_type = ICMP_ECHO;
    icmph->icmp_code = 0;
    icmph->icmp_cksum = 0;
    icmph->icmp_seq = seq;
    icmph->icmp_id = uid & 0xffff;
    for (i = 0; i < length; i++)
    {
	icmph->icmp_data[i] = i;
    }

    icmph->icmp_cksum = icmp_cksum((unsigned char *)icmph, length);
}

static void *icmp_send(void *argv)
{
    gettimeofday(&tv_begin, NULL);

    while(alive)
    {
	int size = 0;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pingm_packet *packet = icmp_findpacket(-1);

	if (packet)
	{
	    packet->seq = packet_send;
	    packet->flag = 1;
	    gettimeofday(&packet->tv_begin, NULL);
	}

	icmp_pack((struct icmp *)send_buff, packet_send, &tv, 64);

	size = sendto(rawsock, send_buff, 64, 0, (struct sockaddr*)&dest, sizeof(dest));
	if (size < 0)
	{
	    perror("sendto()");

	    continue;
	}

	packet_send++;

	sleep(1);
    }
}

/* 解析 ping 封包 */
static int icmp_unpack(char *buf, int len)
{
    int iphdrlen;
    struct ip *ip = NULL;
    struct icmp *icmp = NULL;
    int rtt;

    ip = (struct ip *)buf;
    iphdrlen = ip->ip_hl * 4;
    icmp = (struct icmp *)(buf + iphdrlen);
    len -= iphdrlen;
    
    if (len < 8)
    {
	printf("ICMP packet\'s length is less than 8\n");
	return -1;
    }

    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (uid == icmp->icmp_id))
    {
	struct timeval tv_interval, tv_recv, tv_send;
	pingm_packet *packet = icmp_findpacket(icmp->icmp_seq);
	if (NULL == packet)
	{
	    return -1;
	}

	packet->flag = 0;
	tv_send = packet->tv_begin;
	gettimeofday(&tv_recv, NULL);
	tv_interval = icmp_tvsub(tv_recv, tv_send);
	rtt = tv_interval.tv_sec * 1000 + tv_interval.tv_usec / 1000;
	printf("%d bytes from %s:icmp_seq=%u ttl=%d rtt=%d ms\n",
		len,
		inet_ntoa(ip->ip_src),
		icmp->icmp_seq,
		ip->ip_ttl,
		rtt
		);
	packet_recv ++;
    }
    else
    {
	return -1;
    }

    return 0;
}

static void *icmp_recv(void *argv)
{
    struct timeval tv;
    tv.tv_usec = 200;
    tv.tv_sec = 0;
    fd_set readfd;

    while (alive)
    {
	int ret = 0;
	FD_ZERO(&readfd);
	FD_SET(rawsock, &readfd);

	ret = select(rawsock + 1, &readfd, NULL, NULL, &tv);

	switch (ret)
	{
	    case -1:
		break;
	    case 0:
		break;
	    default:
		{
		    int size = recv(rawsock, recv_buff, sizeof(recv_buff), 0);
		    if (EINTR == errno)
		    {
			perror("recv()");

			continue;
		    }

		    ret = icmp_unpack(recv_buff, size);
		    if (-1 == ret)
		    {
			continue;
		    }

		    break;
		}
	}
    }
}

static void icmp_statistics(void)
{
    long time = tv_interval.tv_sec * 1000 + tv_interval.tv_usec / 1000;
    printf("---- %s ping statistics ----\n", dest_str);
    printf("%d packets transmitted,%d received, %d%% packet loss, time %ldms\n",
	    packet_send,
	    packet_recv,
	    (packet_send - packet_recv) * 100/packet_send,
	    time
	    );
}

int main(int argc, char *argv[])
{
    struct hostent *host = NULL;
    struct protoent *protocol = NULL;
    char protoname[] = "icmp";
    uint32_t inaddr = 1;
    int size = 128 * K;

    pthread_t send_id, recv_id;
    int err = 0;

    if (argc < 2)
    {
	icmp_usage();

	return -1;
    }

    protocol = getprotobyname(protoname);
    if (NULL == protocol)
    {
	perror("getprotobyname()");

	return -1;
    }

    memcpy(dest_str, argv[1], strlen(argv[1]) + 1);
    memset(&pingpacket, 0, sizeof(pingm_packet) * 128);

    /* 初始化原始套接字 */
    rawsock = socket(AF_INET,SOCK_RAW, protocol->p_proto);
    if (rawsock < 0)
    {
	perror("socket()");

	return -1;
    }


    uid = getuid();

    if (setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
    {
	perror("setsockopt()");
	close(rawsock);

	return -1;
    }

    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    inaddr = inet_addr(dest_str);
    if (INADDR_NONE == inaddr)
    {
	host = gethostbyname(dest_str);
	if (NULL == host)
	{
	    perror("gethostbyname()");
	    close(rawsock);

	    return -1;
	}

	memcpy((char *)&dest.sin_addr, host->h_addr, host->h_length);
    }
    else
    {
	memcpy((char *)&dest.sin_addr, &inaddr, sizeof(inaddr));
    }

    inaddr = dest.sin_addr.s_addr;
    printf("PING %s (%d.%d.%d.%d) 56(84) bytes of data.\n",
	    dest_str,
	    (inaddr&0x000000FF)>>0,
	    (inaddr&0x0000FF00)>>8,
	    (inaddr&0x00FF0000)>>16,
	    (inaddr&0xFF000000)>>24
	  );


    signal(SIGINT, icmp_sigint);

    alive = 1;

    err = pthread_create(&send_id, NULL, icmp_send, NULL);
    if (err < 0)
    {
	close(rawsock);

	return -1;
    }

    err = pthread_create(&recv_id, NULL, icmp_recv, NULL);
    if (err < 0)
    {
	close(rawsock);

	return -1;
    }

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);

    icmp_statistics();

    close(rawsock);

    return 0;
}
