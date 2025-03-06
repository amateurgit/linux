#!/bin/bash

source ${TOPDIR}/webapp/.config

curl -k https://${IPADDR}/mgmt/twofactor/email/send?username=test
