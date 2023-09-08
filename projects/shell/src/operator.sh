#!/bin/bash

: <<EOF
 算数运算符
     +, -, *, /, %, ==, !=
 关系运算符(数字)
     -eq, -ne, -gt, -lt, -ge, -le
 布尔运算符
     !, -o, -a
 逻辑运算符
    &&,|| 
 字符串运算符
     =, !=, -z, -n, $
 文件测试运算符

 三元运算符
     :-
EOF

a=10
b=20

echo "------------------算术运算符------------------"
val=`expr $a \* $b`
echo "a * b = $val"



echo "------------------关系运算符(只支持数字)------------------"
if [ $a -eq $b ]
then
    echo "$a -eq $b : a 等于 b"
else
    echo "$a -eq $b : a 不等于 b"
fi

echo "------------------布尔运算符------------------"
if [ $a -lt 100 -a $b -gt 15 ]
then
    echo "$a 小于 100 且 $b 大于 15 : True "
else
    echo "$a 小于 100 且 $b 大于 15 : False "
fi

echo "------------------逻辑运算符------------------"
if [[ $a -lt 100 && $b -gt 15 ]]
then
    echo "$a 小于 100 且 $b 大于 15 : True "
else
    echo "$a 小于 100 且 $b 大于 15 : False "
fi

a="abc"
b="efg"
echo "------------------字符串运算符------------------"
if [ -z $a ]
then
    echo "-z $a 字符串长度为 0"
else
    echo "-z $a 字符串长度不为 0"
fi

unset a
if [ $a ]
then
    echo "$a 字符串不为空"
else
    echo "$a 字符串为空"
fi


echo "------------------文件测试运算符------------------"
file='/root/'
if [ -e $file ]
then
    echo $file" 存在"
fi

: <<EOF
-b file    检测文件是否是块设备文件，如果是，则返回 true。 	[ -b $file ] 返回 false。
-c file    检测文件是否是字符设备文件，如果是，则返回 true。 	[ -c $file ] 返回 false。
-d file    检测文件是否是目录，如果是，则返回 true。 	        [ -d $file ] 返回 false。
-f file    检测文件是否是普通文件(既不是目录，也不是设备文件)，如果是，则返回 true。 	[ -f $file ] 返回 true。
-g file    检测文件是否设置了 SGID 位，如果是，则返回 true。 	[ -g $file ] 返回 false。
-k file    检测文件是否设置了粘着位(Sticky Bit)，如果是，则返回 true。 	[ -k $file ] 返回 false。
-p file    检测文件是否是有名管道，如果是，则返回 true。 	[ -p $file ] 返回 false。
-u file    检测文件是否设置了 SUID 位，如果是，则返回 true。 	[ -u $file ] 返回 false。
-r file    检测文件是否可读，如果是，则返回 true。 	        [ -r $file ] 返回 true。
-w file    检测文件是否可写，如果是，则返回 true。 	        [ -w $file ] 返回 true。
-x file    检测文件是否可执行，如果是，则返回 true。 	        [ -x $file ] 返回 true。
-s file    检测文件是否为空（文件大小是否大于0），不为空返回 true。 	[ -s $file ] 返回 true。
-e file    检测文件（包括目录）是否存在，如果是，则返回 true。 	[ -e $file ] 返回 true。
-S         判断某文件是否 socket。
-L         检测文件是否存在并且是一个符号链接
EOF


echo "------------------ 三元运算符 --------------------------"
echo "${1:-"Hello"}"      # $1 为空, 则取值为 "Hello"
