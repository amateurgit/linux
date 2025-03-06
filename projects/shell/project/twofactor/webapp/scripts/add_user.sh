#!/bin/bash

source ${TOPDIR}/webapp/.config

args="id=&name=test&password=1&cfm_password=1&user_roles=admin&user_device_group=&user_device=APW1KSA0A1000003&trust_hosts=0.0.0.0%2C00:00:00:00:00:00&email=${MAILTO}&two_factor_enable=1"

curl -k -b /tmp/cookie -X POST -d "${args}" https://${IPADDR}/mgmt/user/update
