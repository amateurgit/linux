#!/bin/bash

source ${TOPDIR}/webapp/.config
curl -k -X POST -d "username=test&password=1&captcha=$1" https://${IPADDR}/logincheck
