#!/bin/bash

value=`cat /home/pi/cronPrograms/LoRa/packet`
# echo $value
# 
# FOO=`echo $value | gzip | base64 -w0`
# echo $FOO
# 
# SFOO=`echo $FOO | base64 -d | gunzip`
# echo $SFOO
# 
# ./dragino_lora_app sender $FOO
# ./dragino_lora_app1024 sender $FOO
./dragino_lora_app sender $value
