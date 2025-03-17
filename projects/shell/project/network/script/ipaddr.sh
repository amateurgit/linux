#!/bin/bash

if [ $# -ne 1 ]; then
    echo "用法 : $0 接口名"
    exit 1
fi

ifname=$1

ipaddr_netmask=$(ip addr show ${ifname} | awk '/inet / {print $2}')
echo ${ipaddr_netmask} | awk -F/ '{print $1}'
