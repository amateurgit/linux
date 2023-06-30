/*******************************************************************************
 * Copyright (c), 2023-2033, xxx Tech. Co., Ltd.
 * FileName : ifr.h
 * Author : sjwang    Version : 1.0    Date : 2023-06-30
 * Description :
 * Others :
 * Function List :
 *     1. get_nic_status - get nic status
 * History :
 *     1. Date :
 *        Author :
 *        Modification
*******************************************************************************/

#ifndef __IFR_H__
#define __IFR_H__

typedef struct nic_status 
{
    int up;
    int running;
    int promisc;
} nic_status_t;

/*************************************************
Function: get_nic_status
Description: get nis status
Calls: 
Called By: 
Table Accessed: 
Table Updated:
Input:
    ifname - nic name
Output:
    nic    - nic status
Return: 
    0  - success
    -1 - failure
Others:
*************************************************/
int get_nic_status(const char *ifname, nic_status_t *nic);

#endif
