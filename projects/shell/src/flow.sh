#!/bin/bash

echo "------------ if 条件 ------------"
a=1
b=2
if [ $a == $b ]
then
    echo "相等"
else
    echo "不相等"
fi


echo "------------ for 循环 ------------"
week=("Monday" "Tuesday" "Wednesday" "Thursday" "Friday" "Saturday" "Sunday")
for i in ${week[*]}
do
    echo $i
done

for (( i = 0; i < ${#week[*]}; i++ ));do
    echo "${week[${i}]}"
done

#
#  while 结束条件是 false
#  until 结束条件是 true
#
echo "------------ while 循环 ------------"
n=1
while(($n <= 5))
do
    echo $n
    n=`expr $n + 1`
done

echo "------------ untile 循环 ------------"
n=1
until [ ! $n -lt 10 ]
do
    echo $n
    n=`expr $n + 1`
done


echo "------------ case 分支 ------------"
echo "输入 1 到 4 之间的数字"
echo "你输入的数字为 :"
read num
case $num in
    1) echo '你选择了 1'
    ;;
    2) echo '你选择了 2'
    ;;
    3) echo '你选择了 3'
    ;;
    4) echo '你选择了 4'
    ;;
    *) echo '输入无效'
    ;;
esac
