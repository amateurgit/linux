#/bin/bash

# 语法 :
#
#     getopt 选项定义 参数
#
#        例如 :
#
#            getopt -o ab:c:: --long along,blong:,clong:: -n "$0" -- "$@"
#
#                选项定义 (-o ab:c:: --long along,blong:,clong::) 定义了脚本支持的短选项和长选项
#
#                参数 (-- "$@")  脚本的全部参数传递给 getopt
#
#                注意 :
#                   :  必须参数
#                   :: 可选参数
#                      短选项带可选参数时必须紧贴选项, 例如 -c5;
#                      长选项带可选参数时必须使用等号, 例如 --clong=xxx
#                   -n "$0" 出错时, 提示脚本文件名


# 示例
# getopt.sh -a b1 -c2 file1 file2
# getopt.sh --along --blong=1 --clong=2 file1 file2
# getopt.sh -a -b1 --clong=2 file1 file2
# getopt.sh -a -b 1 -c 2 file1 file2
# getopt.sh --along --blong 1 --clong 2 file1 file2

echo "原始参数列表 : $@ "

# 格式化命令行参数, 选项和参数分离
ARGS=$(getopt -o ab:c:: --long along,blong:,clong:: -n "$0" -- "$@")
if [ $? != 0 ]; then
    echo "Terminating..."
    exit 1
fi
echo "getopt 参数处理结果, 将选项和参数分离: $ARGS"

# 格式化命令行参数, 并分配至位置参数 ($1, $2 ...)
eval set -- "${ARGS}"
echo "执行 set 指令后, \$@ : $@"

while true; do
    case "$1" in
        -a|--along)
            echo "选项 a"
            shift                                 # shift 是对 $1, $2... 的操作, 选项 a 无参数, shift 左移 1 即可
            ;;
        -b|--blong)
            echo "选项 b, 参数 $2"
            shift 2                               # 选项 b 必选参数, 左移 2 
            ;;
        -c|--clong)
            case $2 in
                "")
		            echo "选项 c, 无参数"
                    shift 2 
                    ;;
                *)
                    echo "选项 c, 参数 $2"
                    shift 2
                    ;;
            esac
            ;;
        --)                                       # 选项处理完毕, 退出循环
            shift
            break
            ;;
        *)
            echo "选项无法识别, 程序退出"
            exit 1
            ;;    
    esac
done

# 处理剩余选项
echo  "剩余参数列表 : $@"
echo "\$1=$1"
echo "\$2=$2"

