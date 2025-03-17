#!/bin/bash

ip route show default | awk '/default/ {print $3}'
