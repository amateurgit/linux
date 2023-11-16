#!/bin/bash

# 禁止 ICMP 重定向
for vpn in /proc/sys/net/ipv4/conf/*;
do
    echo 0 > ${vpn}/accept_redirects;
    echo 0 > ${vpn}/send_redirects;
done

# 开启 ip_forward
echo 1 > /proc/sys/net/ipv4/ip_forward

# 禁止 rp_filter
for vpn in /proc/sys/net/ipv4/conf/*;
do
    echo 0 > ${vpn}/rp_filter;
done
