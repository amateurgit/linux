#!/bin/sh

# 物理网卡 接口名列表
ls /sys/class/net/ | grep -v "$(ls /sys/devices/virtual/net/)"
