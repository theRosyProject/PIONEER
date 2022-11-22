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
char* TOPIC = "pcs-chair";
//RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC
RTC_DS3231 RTC;

EspMQTTClient client(
  "NETGEAR_CBE",  //THIS SHOULD BE CalGuest SSID
  "happybug682", // router password, maybe blank for CalGuest?
  "159.223.196.81",  // MQTT Broker server ip
  //"MQTTUsername",   // Can be omitted if not needed
  //"MQTTPassword",   // Can be omitted if not needed
  ESPName      // Client name that uniquely identify your device
);

int ledPin = 0;
int inputInt = HIGH;

const int BUFFER_SIZE = 50;
char buf[BUFFER_SIZE];

/* for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
*/

/* for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
*/

/* for shield 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 7
*/

/* Feather 32u4 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)
*/

/* Feather m0 w/wing 
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
*/

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

  // Start I2C and RTC
  pinMode(ledPin, OUTPUT);
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
}

// connection with the Cloud server
void onConnectionEstablished() {

  //client.subscribe("test", onMessageReceived);
  client.subscribe(TOPIC, onMessageReceived);


  //client.publish("test", String(ESPName) + " joined the channel");
  client.publish(TOPIC, String(ESPName) + " joined the channel");
}

void loop()
{
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
      String myString = Serial.readString();
      digitalWrite(ledPin, LOW);   // sets the LED on
      DateTime now = RTC.now();
      char buf1[20];
      sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d: ",  now.hour(), now.minute(), now.second(), now.month(), now.day(), now.year());
      myString = buf1 + myString;
      Serial.println(myString);
      client.publish(TOPIC, myString);
    }
    else
    {
      Serial.println("Receive failed");
      digitalWrite(ledPin, HIGH);
    }
  }
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
