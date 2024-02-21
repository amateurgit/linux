#!/bin/bash

file="./data"

# while 循环
while read line
do
    echo "${line}"
done < ${file}

# while 循环
#cat ${file} | while read line
#do
#    echo "${line}"
#done

# for 循环
#for line in "$(cat ${file})"
#do
#    echo "${line}"
#done
