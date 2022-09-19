#!/bin/bash
PID=`ps aux | grep goi2c48_py3.py | grep -v grep | awk '{print $2}'`
echo $PID
if [ "$PID" -ne 0 ]; then
   echo "Programma goi2c48_py3.py in funzione"
else
   echo "Programma goi2c48_py3.py assente, rilancio il processo"
   python3 /home/pi/cronPrograms/goi2c48_py3.py &
fi
###########
PID=`ps aux | grep goi2c49_py3.py | grep -v grep | awk '{print $2}'`
echo $PID
if [ "$PID" -ne 0 ]; then
   echo "Programma goi2c49_py3.py in funzione"
else
   echo "Programma goi2c49_py3.py assente, rilancio il processo"
   python3 /home/pi/cronPrograms/goi2c49_py3.py &
fi
###########
PID=`ps aux | grep goi2cBME280_py3.py | grep -v grep | awk '{print $2}'`
echo $PID
if [ "$PID" -ne 0 ]; then
   echo "Programma goi2cBME280_py3.py in funzione"
else
   echo "Programma goi2cBME280_py3.py assente, rilancio il processo"
   python3 /home/pi/cronPrograms/goi2cBME280_py3.py &
fi
###########
