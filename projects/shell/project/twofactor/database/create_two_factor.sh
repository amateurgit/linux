#!/bin/bash

source ${TOPDIR}/database/.config

mysql -u${USERNAME} -p${PASSWORD} < ${TOPDIR}/database/two_factor.sql 2>/dev/null
if [ $? -eq 0 ]; then
    echo "表创建成功"
else
    echo "表创建失败"
fi
