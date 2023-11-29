#!/bin/bash

set -e
#set -x

apps=('')

function get_app_list ()
{
    i=0
    str=''

    subdirs=$(find . -maxdepth 1 -type d)
    for dir in ${subdirs}
    do
        str=$(basename ${dir})
        if [ $str = '.' ]; then
	        continue
        fi
        apps[i]=${str}
        i=$i+1
    done
}

function usage()
{
    str="${apps[*]}"
    echo "语法 : ./inst.sh [ ${str// /|} ]"
}

function inst()
{
    app=$1
    order=$(cat ${app}/order)

    for deb in ${order[*]}
    do
        dpkg -i $app/debs/$deb
    done
}

function main()
{
    # 获取应用名称列表
    get_app_list
    if [ -z ${apps[0]} ]; then
        echo "没有可安装的应用, 程序退出!"
        exit
    fi

    # 参数校验
    if [ $# -ne 1 ]; then
        usage
        exit
    fi

    app=$1

    # 安装程序
    inst $app

    echo "${appname} 已安装完成!"
}

main $@ 
