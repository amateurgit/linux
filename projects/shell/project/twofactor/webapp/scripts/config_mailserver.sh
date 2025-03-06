#!/bin/bash

source ${TOPDIR}/webapp/.config

args="mailserver=${MAILSERVER}&tls=0&port=25&mailfrom=${MAILFROM}&password=${MAILPWD}"

curl -k -b /tmp/cookie -X POST -d "${args}" https://${IPADDR}/mgmt/twofactor/email/set
