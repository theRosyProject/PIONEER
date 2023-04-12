/*

Read from ADC ozone sensors

Read from ADC NO2 sensor

Publish over MQTT

Auth: F. Dallo
Date: 10 Apr 2023, Feltre, IT

*/

// Libraries for ADC
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads1115_OZO;
Adafruit_ADS1115 ads1115_NO2;

// Libraries for handling MQTT
#include "EspMQTTClient.h"
#include <ArduinoJson.h>

// MQTT: ID, server IP, port, username and password
EspMQTTClient client(
  "CMA_Network",
  "cma_2550",
  //"192.168.119.146",  // MQTT Broker server ip M1 Fede su hotspot Motorola
  "159.223.196.81", // MQTT Broker Daniele su Nokia 6
  //"192.168.xxx.xxx",
  "mqttuser",   // Can be omitted if not needed
  "mqttuser",   // Can be omitted if not needed
  "ESP8266_MRG",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

// MQTT: topics
const PROGMEM char* MQTT_SENSOR_TOPIC_DEBUG = "pioneer/0-380-0-MRG/debug";
const PROGMEM char* MQTT_SENSOR_TOPIC_NO2 = "pioneer/0-380-0-MRG/atmosphere/atmospheric_chemistry/no2/free_troposphere/alphasense_1";
const PROGMEM char* MQTT_SENSOR_TOPIC_OZONE = "pioneer/0-380-0-MRG/atmosphere/atmospheric_chemistry/o3/free_troposphere/alphasense_1";

void setup() {
  // add blink
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500); 
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW


  // Open serial for data communication, wait and welcome message
  // Serial.begin(115200);
  // while (!Serial) {
  //   delay(10);
  // }

  // Initialize ADC
  // Serial.println("Init ADC addr48 and addr49");  
  // Serial.println("Both in ADC Range: +/- 4.096V  1 bit = 2mV");
  ads1115_OZO.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  ads1115_OZO.begin(0x48); // reading the OZO sensors
  delay(100);
  ads1115_NO2.setGain(GAIN_ONE);
  ads1115_NO2.begin(0x49);
  delay(100);

    // init the MQTT connection
  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("LoRa/debug", "ESP8266_MRG: I am going offline");  // You can activate the retain flag by setting the third parameter to true
  
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  //client.subscribe("svalbard/#", [](const String & payload) {
  //  Serial.println(payload);
  //});

  // // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  // client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
  //   Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  // });

  // Publish a message to "debug"
  client.publish(MQTT_SENSOR_TOPIC_DEBUG, "ESP8266_MRG connected"); // You can activate the retain flag by setting the third parameter to true

  // // Execute delayed instructions
  // client.executeDelayed(5 * 1000, []() {
  //   client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  // });
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);
  // init loop client MQTT
  client.loop();

  // Reading OZONE sensors 1
  int16_t adc48_0, adc48_1, adc48_2, adc48_3;

  adc48_0 = ads1115_OZO.readADC_SingleEnded(0);
  adc48_1 = ads1115_OZO.readADC_SingleEnded(1);
  adc48_2 = ads1115_OZO.readADC_SingleEnded(2);
  adc48_3 = ads1115_OZO.readADC_SingleEnded(3);
  // formatting the message
  char bufferOZO[50];
  sprintf(bufferOZO, "%02d,%02d,%02d,%02d",
          adc48_0, adc48_1, adc48_2, adc48_3
  );
  //Serial.println(bufferOZO);
  // prepare JSON
  StaticJsonDocument<150> rootOZO;
  // DATA START
  rootOZO["WE1"] = (String)adc48_0;
  rootOZO["AUX1"] = (String)adc48_1;
  rootOZO["WE2"] = (String)adc48_2;
  rootOZO["AUX2"]  = (String)adc48_3;
  // DATA END
  char jsonCharOZO[150];
  serializeJson(rootOZO, jsonCharOZO);    
  client.publish(MQTT_SENSOR_TOPIC_OZONE, jsonCharOZO, true);
  yield();
  
  //delay(1000);

  // Reading NO2 sensor
  int16_t adc49_0, adc49_1, adc49_2, adc49_3;

  adc49_0 = ads1115_NO2.readADC_SingleEnded(0);
  adc49_1 = ads1115_NO2.readADC_SingleEnded(1);
  adc49_2 = ads1115_NO2.readADC_SingleEnded(2);
  adc49_3 = ads1115_NO2.readADC_SingleEnded(3);
  // formatting the message
  char bufferNO2[50];
  sprintf(bufferNO2, "%02d,%02d,%02d,%02d",
          adc49_0, adc49_1, adc49_2, adc49_3          
  );
  //Serial.println(bufferNO2);
  // prepare JSON
  StaticJsonDocument<150> rootNO2;
  // // DATA START
  rootNO2["WE1"] = (String)adc49_0;
  rootNO2["AUX1"] = (String)adc49_1;
  // DATA END
  char jsonCharNO2[150];
  serializeJson(rootNO2, jsonCharNO2);    
  client.publish(MQTT_SENSOR_TOPIC_NO2, jsonCharNO2, true);
  // yield();
  
  delay(4000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

}





