# Program modified by Federico Dallo
# 27 aprile 2021
#
# Used hints of Tony di Cola
#
# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time
import os, sys
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

# Create the file path
file_path = "/home/pi/PIONEER/measurements"
os.chdir(file_path)
today = time.strftime("%Y-%m-%d")
filename="REG_MAR_O3_i2c_addr_49_"+today+".txt"
open(filename, "a")
# os.chmod(filename, 0664) # python 2
os.chmod(filename, 0o664) # python 3

# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Create the ADC object using the I2C bus
ads = ADS.ADS1115(i2c, address=0x49)

# Choose a gain of 1 for reading voltages from 0 to 4.09V.
# Or pick a different gain to change the range of voltages that are read:
#  - 2/3 = +/-6.144V
#  -   1 = +/-4.096V
#  -   2 = +/-2.048V
#  -   4 = +/-1.024V
#  -   8 = +/-0.512V
#  -  16 = +/-0.256V
# See table 3 in the ADS1015/ADS1115 datasheet for more info on gain.
GAIN = 1


# Create single-ended input on channels
chan0 = AnalogIn(ads, ADS.P0)
chan1 = AnalogIn(ads, ADS.P1)
chan2 = AnalogIn(ads, ADS.P2)
chan3 = AnalogIn(ads, ADS.P3)

while True:
    timestr = time.strftime("%Y-%m-%d %H:%M:%S")
    today = time.strftime("%Y-%m-%d")
    
    filename="REG_MAR_O3_i2c_addr_49_"+today+".txt"
    f = open(filename, "a")
    os.chmod(filename, 0o664)

    f.write( timestr + "{:>5} {:>5.3f}".format(chan0.value, chan0.voltage) + "{:>5} {:>5.3f}".format(chan1.value, chan1.voltage) + "{:>5} {:>5.3f}".format(chan2.value, chan2.voltage) + "{:>5} {:>5.3f}".format(chan3.value, chan3.voltage) + '\n')
    f.close()

    time.sleep(1)
