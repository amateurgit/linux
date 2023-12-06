#!/bin/bash

# 比较小数大小

a=3.14
b=3.15

if [ $(echo "$a > $b" | bc) -eq 1 ];then
    echo "$a 大于 $b"
else
    echo "$a 不大于 $b"
fi
