#!/bin/bash                                                                           
          
# 设定默认值
# $1 为空时, a 值取 'Hello'; 否则, a=$1
a=${1:-'Hello'}                                                                       
                                                                                      
echo ${a} 
