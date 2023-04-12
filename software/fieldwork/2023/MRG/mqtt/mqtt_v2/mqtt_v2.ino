/*

Read from ADC ozone sensors

Read from ADC CO2 sensor

Read from two PM2.5 sensors using Software Serial
Info: https://docs.arduino.cc/tutorials/communication/TwoPortReceive

Publish over MQTT

Auth: F. Dallo
Date: 18 Mar 2023, Venice, IT

*/

// Libraries for ADC
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads1115_OZO;
Adafruit_ADS1115 ads1115_CO2;

// Libraries for handling MQTT
#include "EspMQTTClient.h"
#include <ArduinoJson.h>

// MQTT: ID, server IP, port, username and password
EspMQTTClient client(
  "pioneerNet",
  "cicciopasticcio",
  //"192.168.119.146",  // MQTT Broker server ip M1 Fede su hotspot Motorola
  "192.168.43.42", // MQTT Broker Daniele su Nokia 6
  //"192.168.xxx.xxx",
  "mqttuser",   // Can be omitted if not needed
  "mqttuser",   // Can be omitted if not needed
  "MQTT_RICK_1",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

// MQTT: topics
const PROGMEM char* MQTT_SENSOR_TOPIC_DEBUG = "svalbard/debug";
const PROGMEM char* MQTT_SENSOR_TOPIC_PM = "svalbard/pm";
const PROGMEM char* MQTT_SENSOR_TOPIC_PM_1 = "svalbard/pm/pms5003_1";
const PROGMEM char* MQTT_SENSOR_TOPIC_PM_2 = "svalbard/pm/pms5003_2";
const PROGMEM char* MQTT_SENSOR_TOPIC_CO2 = "svalbard/co2";
const PROGMEM char* MQTT_SENSOR_TOPIC_OZONE = "svalbard/ozone";

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

  // Initialize ADC
  Serial.println("Init ADC addr48 and addr49");  
  Serial.println("Both in ADC Range: +/- 4.096V  1 bit = 2mV");
  ads1115_OZO.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  ads1115_OZO.begin(0x48); // reading the OZO sensors
  delay(100);
  ads1115_CO2.setGain(GAIN_ONE);
  ads1115_CO2.begin(0x49);
  // ads1115_49.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  // ads1115_49.begin(0x49); // reading the CO2 sensor

  // Initialize serial port for PM
  Serial.println("Serial port open, now opening sensors' serial ports");
  delay(1000);

  // init the MQTT connection
  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("svalbard/debug", "MQTT_RICK_1: I am going offline");  // You can activate the retain flag by setting the third parameter to true

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
  client.publish(MQTT_SENSOR_TOPIC_DEBUG, "RICK1 connected"); // You can activate the retain flag by setting the third parameter to true

  // // Execute delayed instructions
  // client.executeDelayed(5 * 1000, []() {
  //   client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  // });
}

void loop() {

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
  Serial.println(bufferOZO);
  // prepare JSON
  StaticJsonDocument<150> rootOZO;
  // DATA START
  rootOZO["WE1"] = (String)adc48_3;
  rootOZO["AUX1"] = (String)adc48_2;
  rootOZO["WE2"] = (String)adc48_1;
  rootOZO["AUX2"]  = (String)adc48_0;
  // DATA END
  char jsonCharOZO[150];
  serializeJson(rootOZO, jsonCharOZO);    
  client.publish(MQTT_SENSOR_TOPIC_OZONE, jsonCharOZO, true);
  yield();
  
  delay(1000);

  // Reading CO2 sensor
  int16_t adc49_0, adc49_1, adc49_2, adc49_3;

  adc49_0 = ads1115_CO2.readADC_SingleEnded(0);
  adc49_1 = ads1115_CO2.readADC_SingleEnded(1);
  adc49_2 = ads1115_CO2.readADC_SingleEnded(2);
  adc49_3 = ads1115_CO2.readADC_SingleEnded(3);
  // formatting the message
  char bufferCO2[50];
  sprintf(bufferCO2, "%02d,%02d,%02d,%02d",
          adc49_0, adc49_1, adc49_2, adc49_3          
  );
  Serial.println(bufferCO2);
  // prepare JSON
  StaticJsonDocument<150> rootCO2;
  // // DATA START
  rootCO2["P"] = (String)adc49_3;
  rootCO2["N"] = (String)adc49_2;
  // DATA END
  char jsonCharCO2[150];
  serializeJson(rootCO2, jsonCharCO2);    
  client.publish(MQTT_SENSOR_TOPIC_CO2, jsonCharCO2, true);
  // yield();
  
  delay(1000);

  
  // Start listen on sensor 1
  pmSerial1.listen();  

  if (pmSerial1.available()) {

    PM25_AQI_Data data1;
   
    if (! aqi1.read(&data1)) {
      Serial.println("Could not read from AQI nr.1");
      delay(500);  // try again in a bit!
      //return;
    }

    // Prepare message for the MQTT
    int PM10std  = data1.pm10_standard;
    int PM25std  = data1.pm25_standard;
    int PM100std = data1.pm100_standard;

    int PM10env  = data1.pm10_env;
    int PM25env  = data1.pm25_env;
    int PM100env = data1.pm100_env;

    int prt03 = data1.particles_03um;
    int prt05 = data1.particles_05um;
    int prt10 = data1.particles_10um;
    int prt25 = data1.particles_25um;
    int prt100= data1.particles_100um;
    // formatting the message
    char bufferPM[100];
    sprintf(bufferPM, "%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d",
            PM10std, PM25std, PM100std, 
            PM10env, PM25env, PM100env,
            prt03, prt05, prt10, prt25, prt100
    );
    Serial.println(bufferPM);
    // prepare JSON
    StaticJsonDocument<550> root;
    // DATA START
    root["PM10std"] = (String)PM10std;
    root["PM25std"] = (String)PM25std;
    root["PM100std"] = (String)PM100std;
    root["PM10env"]  = (String)PM10env;
    root["PM25env"]  = (String)PM25env;
    root["PM100env"] = (String)PM100env;
    root["prt03"] = (String)prt03;
    root["prt05"] = (String)prt05;
    root["prt10"] = (String)prt10;
    root["prt25"] = (String)prt25;
    root["prt100"]= (String)prt100;
    // DATA END
    char jsonChar[550];
    serializeJson(root, jsonChar);    
    client.publish(MQTT_SENSOR_TOPIC_PM_1, jsonChar, true);
    yield();
    
  }

  delay(1000);

// Start listen on sensor 2
  pmSerial2.listen();

  if (pmSerial2.available()) {

    PM25_AQI_Data data2;

    if (! aqi2.read(&data2)) {
      Serial.println("Could not read from AQI nr.2");
      delay(500);  // try again in a bit!
      //return;
    }

    // Prepare message for the MQTT
    int PM10std  = data2.pm10_standard;
    int PM25std  = data2.pm25_standard;
    int PM100std = data2.pm100_standard;

    int PM10env  = data2.pm10_env;
    int PM25env  = data2.pm25_env;
    int PM100env = data2.pm100_env;

    int prt03 = data2.particles_03um;
    int prt05 = data2.particles_05um;
    int prt10 = data2.particles_10um;
    int prt25 = data2.particles_25um;
    int prt100= data2.particles_100um;
    // formatting the message
    char bufferPM[100];
    sprintf(bufferPM, "%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d",
            PM10std, PM25std, PM100std, 
            PM10env, PM25env, PM100env,
            prt03, prt05, prt10, prt25, prt100
    );
    Serial.println(bufferPM);
    // prepare JSON
    StaticJsonDocument<550> root;
    // DATA START
    root["PM10std"] = (String)PM10std;
    root["PM25std"] = (String)PM25std;
    root["PM100std"] = (String)PM100std;
    root["PM10env"]  = (String)PM10env;
    root["PM25env"]  = (String)PM25env;
    root["PM100env"] = (String)PM100env;
    root["prt03"] = (String)prt03;
    root["prt05"] = (String)prt05;
    root["prt10"] = (String)prt10;
    root["prt25"] = (String)prt25;
    root["prt100"]= (String)prt100;
    // DATA END
    char jsonChar[550];
    serializeJson(root, jsonChar);    
    client.publish(MQTT_SENSOR_TOPIC_PM_2, jsonChar, true);
    yield();

    

  }

  delay(1000);

}





