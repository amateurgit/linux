#!/bin/bash

function add()
{
    a=$1
    b=$2
    return $(($a+$b))
}

add 1 2

echo "两个数字之和为 $? !"
