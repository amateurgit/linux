#!/bin/bash

set -e

if [ $# -ne 1 ]
then
    echo "用法 : ./apt-lrn.sh app"
    exit
fi

app=$1

rm -rf $app
mkdir -p $app
mkdir -p $app/debs
touch $app/order

apt-get update

apt-get clean

# 开始学习
apt-get install -y $app > $app/.aptlog
cp /var/cache/apt/archives/*.deb $app/debs
cat $app/.aptlog | grep "Preparing to unpack" | sed -e 's/^.*\/[0-9]\+-\(.*\.deb\).*$/\1/g' > $app/order

echo "已成功学习到安装 ${app} 需要的 debs 和 依赖关系!"
