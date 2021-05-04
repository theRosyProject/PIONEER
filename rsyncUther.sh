#!/bin/bash
#
#	PROGRAMMA PER SINCRONIZZARE I DATI DI OZONO
#	SU UTHER, SERVER OPENVPN A VENEZIA
#
# BEGIN(VARIABILI)*da modificare (BASH non ha le variabili di ambiente!)
USER=pi
PROG_NAME=rsync_to_Uther
# Le seguenti sono normalmente costanti
DATE=`date +%Y-%m-%d`
WORKDIR=/home/$USER/$PROG_NAME
PIDFILE=/home/$USER/$PROG_NAME.pid
#
# dumb ##MRG_O3_i2c_addr_49_2019-10-16.txt
FILE_48="REG_MAR_O3_i2c_addr_48_"$DATE".txt"
FILE_49="REG_MAR_O3_i2c_addr_49_"$DATE".txt"
FILE_BME="REG_MAR_BME280_i2c_"$DATE".txt"
#FILE_22="MRG_T_RH_gpio22_sensor_"$DATE".txt"
#FILE_23="MRG_T_RH_gpio23_sensor_"$DATE".txt"
#
#FILENAME=calibration_addr_48_$DATE.txt
#FILENAME=
# END(VARIABILI)
#
if [ -f $PIDFILE ]
then
    PID=$(cat $PIDFILE)
    ps -p $PID > /dev/null 2>&1
    if [ $? -eq 0 ]
    then
        echo "$PROG_NAME job is already running"
        exit 1
    else
        ## As process not found let assume is not running
        echo $$ > $PIDFILE
        if [ $? -ne 0 ]
        then
            echo "Could not create PID file for $PROG_NAME"
            exit 1
        fi
    fi
else
    echo $$ > $PIDFILE
    if [ $? -ne 0 ]
    then
        echo "Could not create PID file for $PROG_NAME"
        exit 1
    fi
#####BEGIN(COMMANDS)
    echo "$PROG_NAME started: `date`"
    cp /home/pi/PIONEER/measurements/$FILE_48 /home/pi/PIONEER/measurements/toSync/
    cp /home/pi/PIONEER/measurements/$FILE_49 /home/pi/PIONEER/measurements/toSync/
    cp /home/pi/PIONEER/measurements/$FILE_BME /home/pi/PIONEER/measurements/toSync/
    #cat /home/pi/o3net/measurements/localdata_addr_48.txt >> /home/pi/o3net/measurements/ozono_all_station1_addr_48.txt && > /home/pi/o3net/measurements/localdata_addr_48.txt
    #rclone copy /home/pi/o3net/measurements/T_RH_all_station1.txt gdrive:OZO/
    rsync -avui /home/pi/PIONEER/measurements/toSync/ kopu@10.8.0.1:./OZO/toSync
    rm /home/pi/PIONEER/measurements/toSync/*
    echo "$PROG_NAME finished: `date`"
#####END(COMMANDS)
fi
rm $PIDFILE
