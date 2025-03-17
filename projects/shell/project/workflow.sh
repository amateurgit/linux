#!/bin/sh

export TOPDIR=$(pwd)

# 定义选项
ARGS=$(getopt -o h --long help,show-if-info -n "$0" -- "$@")
if [ $? -ne 0 ]; then
    exit 1
fi

eval set -- "${ARGS}"

while true; do
    case "$1" in
        -h|--help)
            ${TOPDIR}/help/usage.sh
            shift
            ;;

        --show-if-info)                # 显示网卡信息
            ${TOPDIR}/network/show_if_info.sh
            shift
            ;;

        --)
            shift
            break
            ;;

        *)
            echo "内部错误"
            break
            ;;
    esac
done
