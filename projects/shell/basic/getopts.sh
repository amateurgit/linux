#!/bin/sh

# 语法
#    getopts optstring name [arg...]
#
#        optstring - 定义脚本支持的选项, 例如 : "a:b"。冒号表示参数。
#
#        name      - 下一个要处理的选项
#
#        arg       - 处理的选项列表, 缺省值 $@



# 示例 : 
# 执行命令 ./getopts.sh -a 12 -b -c file1 file2

echo "原始参数列表: [ $@ ]"
echo "选项索引 OPTIND 存储原始参数列表中下一个要处理的参数位置, 初始值: $OPTIND"

optstring="a:bc"

while getopts ${optstring} opt; do
    case ${opt} in
        a)
            echo "这是 -a 选项. 选项参数 OPTARG : ${OPTARG}, 选项索引 OPTIND : ${OPTIND}"
            ;;
        b)
            echo "这是 -b 选项. 选项参数 OPTARG : ${OPTARG}, 选项索引 OPTIND : ${OPTIND}"
            ;;
        c)
            echo "这是 -c 选项. 选项参数 OPTARG : ${OPTARG}, 选项索引 OPTIND : ${OPTIND}"
            ;;
        ?)
            echo "选项无法识别导致程序退出"
            exit 1
            ;;

    esac
done

shift $((${OPTIND} - 1))
echo "移除选项后剩余的参数列表 [ $@ ]"
echo "\$1=[ $1 ]"
echo "\$2=[ $2 ]"
