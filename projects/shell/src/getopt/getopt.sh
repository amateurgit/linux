#/bin/bash

echo "original parameters=[$@]"

# -o      - 定义短选项
# --long  - 定义长选项
# -n      - 出错时, 提示脚本文件名
# -- "$@" - 列出所有参数
ARGS=`getopt -o ab:c:: --long along,blong:,clong:: -n "$0" -- "$@"`
if [ $? != 0 ]; then
    echo "Terminating..."
    exit 1
fi
echo ARGS=[$ARGS]

# 格式化命令行参数, 并分配至位置参数 ($1, $2 ...)
eval set -- "${ARGS}"

echo formatted parameters=[$@]

while true; do
    case "$1" in
        -a|--along)
            echo "Option a"
            shift                                 # shift 是对 $1, $2... 的操作
            ;;
        -b|--blong)
            echo "Option b, argument $2"
            shift 2
            ;;
        -c|--clong)
            case $2 in
                "")
		    echo "Option c, no argument"
                    shift 2
                    ;;
                *)
                    echo "Option c, argument $2"
                    shift 2
                    ;;
            esac
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "Internal error!"
            exit 1
            ;;    
    esac
done

# 处理剩余选项
echo remaining parameters=[$@]
echo \$1=[$1]
echo \$2=[$2]

# bash getopt.sh -a b1 -c2 file1 file2
# bash getopt.sh --along --blong=1 --clong=2 file1 file2
# bash getopt.sh -a -b1 --clong=2 file1 file2
# bash getopt.sh -a -b 1 -c 2 file1 file2
# bash getopt.sh --along --blong 1 --clong 2 file1 file2
