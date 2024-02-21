#!/bin/bash

date="2000-01-01 00:00:00"

timestamp=$(date -d "${date}" +%s)

echo ${timestamp}
