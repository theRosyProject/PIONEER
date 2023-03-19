/*

Read from two PM2.5 sensors using Software Serial
Info: https://docs.arduino.cc/tutorials/communication/TwoPortReceive

Auth: F. Dallo
Date: 19 Mar 2023, Venice, IT

*/

// Add libraries for handlind OTA
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// This is the project WiFi credentials
const char* ssid = "pioneerNet";
const char* password = "cicciopasticcio";

// Debugging LED
const int ESP_BUILTIN_LED = 2;

// Add the library from read from PMS5003 sensors
#include "Adafruit_PM25AQI.h"

// Add the library for using two sensors over software serial port
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

  // Start the WiFi configuration
  Serial.println("WiFi booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Start the OTA configuration
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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

  digitalWrite(ESP_BUILTIN_LED, LOW);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, HIGH);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, LOW);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, HIGH);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, LOW);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, HIGH);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, LOW);
  delay(200);
  digitalWrite(ESP_BUILTIN_LED, HIGH);

}

void loop() {

  // Start OTA hantler
  ArduinoOTA.handle();
  
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

    digitalWrite(ESP_BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(ESP_BUILTIN_LED, HIGH);
  
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

    digitalWrite(ESP_BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(ESP_BUILTIN_LED, HIGH);

  }

  delay(5000);

}





