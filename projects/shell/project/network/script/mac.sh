#!/bin/bash

if [ $# -ne 1 ]; then
    echo "用法 : $0 接口名"
    exit 1
fi

ifname=$1

ip addr show ${ifname} | grep 'link/ether' | awk '{print $2}'
