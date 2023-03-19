#/bin/bash

mosquitto_pub -h 192.168.119.146 -t "svalbard/pm25" -m "sensor1, 30"
