#!/bin/bash

#
# 1. 变量赋值, 变量名和等号之间不能有空格
# 2. 变量引用, 加大括号
#

# 只读属性
name="Wang Shijie"
readonly name
echo ${name}

# unset
hello="Hello world!"
echo ${hello}
unset hello
echo ${hello}

# 单引号与双引号
#     - 双引号中可以包含变量和转义字符
#     - 单引号原样输出, 不能包含变量和转义字符
str="I know you are \"${name}\"!\n"
echo ${str}

# 字符串长度
echo ${#name}
