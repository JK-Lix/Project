#!/bin/bash
cd /home/flash/master/

while true; do
    server=`ps -aux | grep pihealthd | grep -v grep`
    if [ ! "$server" ]; then
        nohup /home/flash/master/pihealthd -c 1 &
        sleep 30
    fi
    sleep 30
done
