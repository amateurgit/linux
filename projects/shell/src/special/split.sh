#!/bin/bash

STR="/home/xxx/hello.c"

echo "原字符串 : ${STR}, key : /"

echo "#* : 从左到右最小匹配, 删除左侧"
substr=${STR#*/}
echo ${substr}

echo "##* : 从左到右最大匹配, 删除左侧"
substr=${STR##*/}
echo ${substr}

echo "#* : 从右到左最小匹配, 删除右侧"
substr=${STR%/*}
echo ${substr}

echo "#* : 从右到左最大匹配, 删除右侧"
substr=${STR%%/*}
echo ${substr}

