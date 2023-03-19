#!/bin/bash
filename=./data/$(date +"%Y_%m_%d_%I_%M_%S")_svalbard.log
echo $filename
echo "Lunching log for topic 'svalbard'"
mosquitto_sub -h 192.168.119.146 -t "svalbard/#" -F '{ "brokerTime":"%I", "topic":"%t", "payload": %p }' >> $filename 
