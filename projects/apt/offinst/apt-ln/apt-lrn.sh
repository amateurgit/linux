
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

# 学习
export DEBIAN_FRONTEND=noninteractive
apt-get install -yq $app > $app/.aptlog
cp /var/cache/apt/archives/*.deb $app/debs

# 依赖包列表 debs
tmpdebs=$(find ${app}/debs/ -name *.deb)
i=0
for deb in ${tmpdebs[*]};do
    debs[i]=$(basename ${deb})
    i=${i}+1
done

# 依赖顺序 order
cat $app/.aptlog | grep "Preparing to unpack" > $app/.odr
while read line;do
    for deb in ${debs[*]};do
        if [[ "${line}" =~ "${deb}" ]];then
            echo ${deb} >> $app/order
        fi
    done
done < $app/.odr
rm -rf $app/.odr

echo "已成功学习到安装 ${app} 需要的 debs 和 依赖关系!"

