// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>

// Libraries and code for sending data on the cloud via MQTT
#include "EspMQTTClient.h"
#include <Wire.h> // Library for I2C communication
#include "RTClib.h"

char* ESPName = "Gate_1";
char* TOPIC = "LoRa";

RTC_DS3231 rtc;

EspMQTTClient client(
  "NETGEAR_CBE",  //THIS SHOULD BE CalGuest SSID
  "happybug682", // router password, maybe blank for CalGuest?
  "159.223.196.81",  // MQTT Broker server ip
  //"MQTTUsername",   // Can be omitted if not needed
  //"MQTTPassword",   // Can be omitted if not needed
  ESPName      // Client name that uniquely identify your device
);

// save time into variable for the keep alive message to send to the broker
//uint8_t previousHour;

#if defined(ESP8266)
  /* for ESP w/featherwing */ 
  #define RFM95_CS  2    // "E"
  #define RFM95_RST 16   // "D"
  #define RFM95_INT 15   // "B"

#elif defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)
 // Feather M0 w/Radio
  #define RFM95_CS      8
  #define RFM95_INT     3
  #define RFM95_RST     4

#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) || defined(ARDUINO_NRF52840_FEATHER) || defined(ARDUINO_NRF52840_FEATHER_SENSE)
  #define RFM95_INT     9  // "A"
  #define RFM95_CS      10  // "B"
  #define RFM95_RST     11  // "C"

#elif defined(ESP32)  
  /* ESP32 feather w/wing */
  #define RFM95_RST     27   // "A"
  #define RFM95_CS      33   // "B"
  #define RFM95_INT     12   //  next to A

#elif defined(ARDUINO_NRF52832_FEATHER)
  /* nRF52832 feather w/wing */
  #define RFM95_RST     7   // "A"
  #define RFM95_CS      11   // "B"
  #define RFM95_INT     31   // "C"
  #define LED           17
  
#elif defined(TEENSYDUINO)
  /* Teensy 3.x w/wing */
  #define RFM95_RST     9   // "A"
  #define RFM95_CS      10   // "B"
  #define RFM95_INT     4    // "C"
#endif


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 13

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);

  Serial.println("Feather LoRa RX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  // Start I2C
  Serial.println("Start I2C");
  //pinMode(ledPin, OUTPUT);
  Wire.begin(); // Start the I2C

  // Start RTC
  Serial.println("Start RTC"); 

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  DateTime now = rtc.now();
  char starttime[20];
  sprintf(starttime, "%02d/%02d/%02d %02d:%02d:%02d ",  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );
  Serial.println(starttime); 

  // keep track of the current hour (will be hour? day? in the final project)
  //previousHour = now.hour();
}

// connection with the Cloud server
void onConnectionEstablished() {

  Serial.println("Connection Established");

  //client.subscribe("test", onMessageReceived);
  client.subscribe(TOPIC, onMessageReceived);

  // get time from RTC clock
  DateTime now = rtc.now();
  char starttime[20];
  sprintf(starttime, "%02d/%02d/%02d %02d:%02d:%02d ",  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );

  //client.publish("test", String(ESPName) + " joined the channel");
  client.publish(TOPIC, String(starttime) + String(ESPName) + " joined the channel");
}

void loop()
{
  // start the mqtt loop
  client.loop();

  // save the current time into a variable
  //DateTime alive_now = rtc.now();

  // debug to send serial message over the MQTT
  // if (Serial.available() > 0) {
  //   String myString = Serial.readString();
  //   digitalWrite(ledPin, LOW);   // sets the LED on

  //   DateTime now = rtc.now();
  //   char buf1[20];
  //   sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d: ",  now.hour(), now.minute(), now.second(), now.month(), now.day(), now.year());
  //   myString = buf1 + myString;
  //   Serial.println(myString);
  //   //client.publish("test", myString);
  //   client.publish(TOPIC, myString);
  // } else {
  //   digitalWrite(ledPin, HIGH);
  // }

  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);

      // Send data on the cloud
      //String myString = Serial.readString();
      String myString = (char*)buf;
      //digitalWrite(ledPin, LOW);   // sets the LED on
      DateTime now = rtc.now();
      char buf1[20];
      sprintf(buf1, "%02d/%02d/%02d %02d:%02d:%02d ",  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );
      myString = buf1 + myString;
      Serial.println(myString);
      client.publish(TOPIC, myString);
    }
    else
    {
      Serial.println("Receive failed");
      //digitalWrite(ledPin, HIGH);
    }
  }

  // keep alive message
  // if ( alive_now.hour() != previousHour )
  // {
  //   DateTime nalive_now = rtc.now();
  //   previousHour = alive_now.hour();
  //   char starttime[20];
  //   sprintf(starttime, "%02d/%02d/%02d %02d:%02d:%02d ",  alive_now.year(), alive_now.month(), alive_now.day(), alive_now.hour(), alive_now.minute(), alive_now.second() );
  //   client.publish(TOPIC, String(starttime) + String(ESPName) + " alive");
  // }

}

void onMessageReceived(const String& message) {
  Serial.println("message received: " + message);
//  if (message.substring(0, message.indexOf(":")) == ESPName) {
//    String newSetting = message.substring(message.indexOf(":") + 1);
//    Serial.println(newSetting)
//    if (newSetting.substring(0, newSetting.indexOf(":")) == "TIME") {
//      timeString = newSetting.substring(0, newSetting.indexOf(":"))
//      hour = .toInt()
////      rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
//      rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
//      rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
//    }
//  }
}
