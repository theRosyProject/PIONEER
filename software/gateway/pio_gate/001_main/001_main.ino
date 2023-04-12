// This file contains Libraries and Global Variables

// Libraries for the SPI protocol
#include <SPI.h>

// Libraries for using the LoRa Radio module
#include <RH_RF95.h>

// Libraries for sending data on the cloud via MQTT
#include "EspMQTTClient.h"

// Libraries for I2C communication 
#include <Wire.h> 

// Libraries for using the Real Time Clock module
#include "RTClib.h"

// Libraries for datalogging (for the Node only)
#include <SD.h>

// ESP8266 configuration variables
char* ESPName = "Gate_0000"; // This value should be unique

// ESP8266 LoRa configuration
/* for ESP w/featherwing */ 
#define RFM95_CS  2    // "E"
#define RFM95_RST 16   // "D"
#define RFM95_INT 15   // "B"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 13

// MQTT configuration variables
char* TOPIC = "LoRa";

EspMQTTClient client(
  //"NETGEAR_CBE",  // SSID
  "AlexPcNetwork_2.4_EXT",  // SSID
  //"happybug682", // password
  "soranzenvasfeltre", // password
  "159.223.196.81",  // MQTT Broker server ip (fixed)
  //"MQTTUsername",   // Can be omitted if not needed
  //"MQTTPassword",   // Can be omitted if not needed
  ESPName      // Client name that uniquely identify your device
);

// RTC configuration variables
RTC_DS3231 rtc;

