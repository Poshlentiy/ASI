#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <ip> <port>"
    exit 1
fi


ip=$1
port=$2

./app/client $ip $port ./Haru1 &
./app/client $ip $port ./Haru &
./app/client $ip $port ./Haru2 &
./app/client $ip $port ./Haru3 &

