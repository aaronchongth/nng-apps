#!/bin/bash

ADDR=ipc:///tmp/async_demo
COUNT=10

source /opt/ros/foxy/setup.bash

ros2 run nng-apps server $ADDR &
SERVER_PID=$!
trap "kill $SERVER_PID" 0
typeset -a CLIENT_PID
i=0
sleep 1
while (( i < COUNT ))
do
	i=$(( i + 1 ))
	rnd=$(( RANDOM % 1000 + 500 ))
	echo "Starting client $i: server replies after $rnd msec"
  source /opt/ros/foxy/setup.bash
	ros2 run nng-apps client $ADDR $rnd &
	eval CLIENT_PID[$i]=$!
done

i=0
while (( i < COUNT ))
do
	i=$(( i + 1 ))
	wait ${CLIENT_PID[$i]}
done
kill $SERVER_PID
