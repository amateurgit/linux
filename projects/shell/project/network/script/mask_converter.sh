#!/bin/bash

netmask_to_cidr() {
    local IFS
    local -a octets    # -a 声明字节数组
    local i=0
    local cidr=0

    IFS='.' read -r -a octets <<< "$1"  # -r : readonly; <<< : 单行输入.
    for octet in "${octets[@]}"; do
        while [ $octet -gt 0 ]; do
            cidr=$((cidr + (octet & 1)))
            octet=$((octet >> 1))
        done
    done

    echo ${cidr}
}

cidr_to_netmask() {
    local cidr=$1
    local shift=$((cidr))
    local netmask=""

    for ((i=0; i<4; i++)); do
        if [ $shift -ge 8 ]; then
            netmask+="255"
        else
            result=$((256 - 2 ** (8 - shift) ))
            netmask+=$(expr $result % 256)
        fi
        shift=$((shift-8))
        [ $i -lt 3 ] && netmask+="."
    done

    echo "$netmask"
}

if [ "$1" -eq "$1" ] 2>/dev/null; then
    cidr_to_netmask $1
else
    netmask_to_cidr $1
fi
