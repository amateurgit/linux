#!/bin/bash

if [ $# -ne 1 ]; then
    echo "用法 : $0 接口名"
    exit 1
fi

ifname=$1

nmcli -v > /dev/null 2>&1
if [ $? -eq 0 ]; then
    nmcli device show ${ifname} | grep DNS | awk '{print $2}'
    exit 0
fi

resolvectl --version > /dev/null 2>&1
if [ $? -eq 0 ]; then
    resolvectl dns ens33 | awk '{print $4}'
    exit 0
fi

systemd-resolve --version > /dev/null 2>&1
if [ $? -eq 0 ]; then
    systemd-resolve --status | grep " DNS Servers" | awk '{print $3}'
    exit 0
fi

if [ -e /etc/resolv.conf ]; then
    dns=$(cat /etc/resolv.conf | grep "nameserver" | awk '{print $2}')
    if [ -e /run/systemd/resolve/resolv.conf ]; then
        dns=$(cat /run/systemd/resolve/resolv.conf | grep "nameserver" | awk '{print $2}')
    fi

    echo ${dns}

    exit 0
fi

echo "查询失败"

exit 1
