/*

Read from two PM2.5 sensors using Software Serial
Info: https://docs.arduino.cc/tutorials/communication/TwoPortReceive

Auth: F. Dallo
Date: 17 Mar 2023, Venice, IT

*/

// Add the library from read from PMS5003 sensors
#include "Adafruit_PM25AQI.h"

#include <SoftwareSerial.h>

SoftwareSerial pmSerial1(2, 0);
SoftwareSerial pmSerial2(13, 12);

Adafruit_PM25AQI aqi1 = Adafruit_PM25AQI();
Adafruit_PM25AQI aqi2 = Adafruit_PM25AQI();

void setup() {
  // Open serial for data communication, wait and welcome message
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("Serial port open, now opening sensors' serial ports");
  delay(1000);

  // Start the sensors serial ports
  
  pmSerial2.begin(9600);

  if (! aqi2.begin_UART(&pmSerial2)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor 2!");
    while (1) delay(10);
  }

  pmSerial1.begin(9600);

  if (! aqi1.begin_UART(&pmSerial1)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor 1!");
    while (1) delay(10);
  }

}

void loop() {
  
  // Start listen on sensor 1
  pmSerial1.listen();  

  if (pmSerial1.available()) {

    PM25_AQI_Data data1;
   
    if (! aqi1.read(&data1)) {
      Serial.println("Could not read from AQI nr.1");
      delay(500);  // try again in a bit!
      //return;
    }

    Serial.print(F("Sensor 1: "));
    Serial.print(F("PM 1.0: ")); Serial.print(data1.pm10_standard);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data1.pm25_standard);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data1.pm100_standard);
  
  }

  delay(5000);

// Start listen on sensor 2
  pmSerial2.listen();

  if (pmSerial2.available()) {

    PM25_AQI_Data data2;

    if (! aqi2.read(&data2)) {
      Serial.println("Could not read from AQI nr.2");
      delay(500);  // try again in a bit!
      //return;
    }

    Serial.print(F("Sensor 2: "));
    Serial.print(F("PM 1.0: ")); Serial.print(data2.pm10_standard);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data2.pm25_standard);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data2.pm100_standard);

  }

  delay(5000);

}





