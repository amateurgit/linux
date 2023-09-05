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

