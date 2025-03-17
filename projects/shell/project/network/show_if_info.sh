#!/bin/bash

SCRIPT=${TOPDIR}/network/script

__show_if_info() {

    ifname=$1

    state=$(${SCRIPT}/state.sh ${ifname})
    mac=$(${SCRIPT}/mac.sh ${ifname})
    ipaddr=$(${SCRIPT}/ipaddr.sh ${ifname})
    netmask=$(${SCRIPT}/netmask.sh ${ifname})
    gateway=$(${SCRIPT}/default_gateway.sh)
    dns=$(${SCRIPT}/DNS.sh ${ifname})

    echo "${ifname} 接口信息"
    if [ "${state}" == "DOWN" ]; then

cat << EOF
    状态 : ${state}
    MAC  : ${mac}

EOF

    else

cat << EOF
    状态 : ${state}
    MAC  : ${mac}
    IP   : ${ipaddr}
    掩码 : ${netmask} | $(${SCRIPT}/mask_converter.sh ${netmask})
    网关 : ${gateway}
    DNS  : ${dns}

EOF

    fi


}


for i in $(${SCRIPT}/phy_ifnames.sh)
do
    __show_if_info $i
done
