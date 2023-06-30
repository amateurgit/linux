/*******************************************************************************
 * Copyright (c), 2023-2033, xxx Tech. Co., Ltd.
 * FileName : if_stats.c
 * Author : sjwang
 * Version : 1.0
 * Date : 2023-06-29
 * Description : 网卡状态
 * Function List :
 * 
 * History :
 *     <author>  <time>    <version>         <desc>
*******************************************************************************/
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <linux/if.h>
#include <sys/ioctl.h>

#include "ifr.h"

int get_nic_status(const char *ifname, nic_status_t *nic)
{
    int sock;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
	return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
    {
	close(sock);
	return -1;
    }
    close(sock);

    if (ifr.ifr_flags & IFF_UP)
    {
	nic->up = 1;
    }

    if (ifr.ifr_flags & IFF_RUNNING)
    {
	nic->running = 1;
    }

    if (ifr.ifr_flags & IFF_PROMISC)
    {
	nic->promisc = 1;
    }

    return 0;
}
