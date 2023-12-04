#!/bin/awk

# BEGIN 与 END 用法
# awk -f sum.awk score

# 文件处理前
BEGIN {
    math=0
    english=0
    computer=0
    printf "NAME    NO.     MATH    ENGLISH     COMPUTER    TOTAL\n"
    printf "-----------------------------------------------------\n"
}

# 文件处理
{
    math+=$3
    english+=$4
    computer+=$5
    printf "%-6s %-6s %4d %8d %8d %8d\n", $1, $2, $3, $4, $5, $3+$4+$5
}

#文件处理后
END {
    printf "-----------------------------------------------------\n"
    printf "AVERAGE :  %10.2f %8.2f %8.2f\n", math/NR, english/NR, computer/NR
}
