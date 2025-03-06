#!/bin/bash

source ${TOPDIR}/webapp/.config

curl -k -c /tmp/cookie -X POST -d "username=${USERNAME}&password=${PASSWORD}" https://${IPADDR}/logincheck
