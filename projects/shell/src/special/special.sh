#!/bin/bash                                                                           
          
# 设定默认值
# $1 为空时, a 值取 'Hello'; 否则, a=$1
a=${1:-'Hello'}                                                                       
                                                                                      
echo ${a} 


#################################################
# 
#  var 被设置, 取 + 后面的值; 否则取默认值 : 空
#
#################################################
var="hello"

result="${var+set}"

echo ${result}

unset var

result="${var+set}"

echo ${result}

