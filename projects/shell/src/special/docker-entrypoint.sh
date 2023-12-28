#!/bin/sh
set -e

# ${1#-} : 参数 1 去掉一个 '-' 前缀, 判断第一个参数是否表示选项
if [ "$#" -eq 0 ] || [ "${1#-}" != "$1" ]; then
# set -- freeradius "$@" : 设置命令行参数, "freeradius" 作为第一个参数,加入到原来的命令行参数列表中
    set -- freeradius "$@"
fi

# check for the expected command
if [ "$1" = 'freeradius' ]; then
    shift
    exec freeradius -f "$@"
fi

# many people are likely to call "radiusd" as well, so allow that
if [ "$1" = 'radiusd' ]; then
    shift
    exec freeradius -f "$@"
fi

# else default to run whatever the user wanted like "bash" or "sh"
exec "$@"
