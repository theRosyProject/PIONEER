# Program by Federico Dallo
# 27 aprile 2021

import os, sys
import board
import digitalio
import time
import adafruit_bme280

# Create the file path
file_path = "/home/pi/PIONEER/measurements"
os.chdir(file_path)
today = time.strftime("%Y-%m-%d")
filename="REG_MAR_BME280_i2c_"+today+".txt"
open(filename, "a")
# os.chmod(filename, 0664) # python 2
os.chmod(filename, 0o664) # python 3

# Create sensor object, using the board's default I2C bus.
i2c = board.I2C()   # uses board.SCL and board.SDA
bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c)
#or with other sensor address
#bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, address=0x76)

# change this to match the location's pressure (hPa) at sea level
bme280.sea_level_pressure = 1013.25

while True:
    timestr = time.strftime("%Y-%m-%d %H:%M:%S")
    today = time.strftime("%Y-%m-%d")
    
    filename="REG_MAR_BME280_i2c_"+today+".txt"
    f = open(filename, "a")
    os.chmod(filename, 0o664)

    f.write( timestr + " Celsius %0.1f" % bme280.temperature + " RH %0.1f" % bme280.relative_humidity + " hPa %0.1f" % bme280.pressure + " Alt_meters %0.1f" % bme280.altitude + '\n')
    f.close()
    
    time.sleep(1)
