#!/bin/bash

: <<EOF
1. 数组
2. 关联数组
EOF

# 数组
week=("Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday" "Sunday")

# 关联数组
declare -A month=(["1"]="January" ["2"]="February" ["3"]="March" ["4"]="April" ["5"]="May" ["6"]="June"
       ["7"]="July" ["8"]="August" ["9"]="September" ["10"]="October" ["11"]="November" ["12"]="December")

echo ${week[0]}       # 获取某个元素
echo ${month['1']}

echo ${week[*]}       # 获取所有元素
echo ${month[*]}

echo ${!week[*]}      # 获取所有索引或键值
echo ${!month[*]}

echo ${#week[*]}      # 获取长度
echo ${#month[*]}
