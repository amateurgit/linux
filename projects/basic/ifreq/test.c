/*******************************************************************************
 * Copyright (c), 2023-2033, xxx Tech. Co., Ltd.
 * FileName : test.c
 * Author : sjwang
 * Version : 1.0
 * Date : 2023-06-30
 * Description : 
 *     gcc test.c ifr.c -o nic_status 
 * Function List :
 *     get_nic_status
 * History :
 *     <author>  <time>    <version>         <desc>
 *      sjwang   23/06/29     1.0       build this module
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "ifr.h"

static void print_nic(nic_status_t *nic)
{
    printf("up      : %d\n", nic->up);
    printf("running : %d\n", nic->running);
    printf("promisc : %d\n", nic->promisc);

    return;
}

int main(int argc, char *argv[])
{
    nic_status_t nic = { 0 };
    int ret = -1;

    if (argc != 2)
    {
	printf("usage : ./nic_status ifname\n");

	exit(EXIT_FAILURE);
    }

    /* 获取网卡状态 */
    ret = get_nic_status(argv[1], &nic);
    if (ret < 0)
    {
	perror("get_nic_status");
	exit(EXIT_FAILURE);
    }

    /* 打印网卡状态 */
    print_nic(&nic);

    exit(EXIT_SUCCESS);
}
