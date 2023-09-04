#!/bin/bash

:<<EOF
命令
    ./script_args.sh 1 2 3
EOF

# 传参
echo "脚本文件: "${0}
echo "第一个参数: "${1}
echo "第二个参数: "${2}
echo "第三个参数: "${3}

# 符号变量
echo "参数个数 : "$#

echo "所有参数 : "$*
echo "所有参数 : "$@
echo "-- \$* 演示 ---"
for i in "$*";do
    echo $i
done
echo "-- \$@ 演示 ---"
for i in "$@";do
    echo $i
done

echo "进程ID : "$$

echo "后台运行的最后一个进程ID : "$!

#
# shell 选项可以控制 shell 的执行方式
# set 命令可以设置 shell 选项
# i - 交互式 shell
#
echo "shell 选项 : "$-

echo "最后命令的退出状态 : "$?
