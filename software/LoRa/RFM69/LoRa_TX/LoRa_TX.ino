// LoRa module libraries
#include <SPI.h>
#include <RH_RF95.h>

char* ESPName = "Node_1";

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

// Read message from serial and send over LoRa
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// RTC 
#include "RTClib.h"
RTC_DS3231 rtc;

// BME280 libs and config
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
unsigned long delayTime;

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  delay(100);

  Serial.println("Feather LoRa TX Test!");

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

  // Start RTC
  Serial.println("Start RTC");
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

  // BME280 test
  Serial.println(F("BME280 test"));
  unsigned status;
  status = bme.begin();
  if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
  }

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // Write in console that system is ready
  Serial.println("End Setup. Console ready");
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  // send through LoRa a string when a newline arrives:
  if (stringComplete) {
    Serial.println("Transmitting..."); // Send a message to rf95_server
    Serial.println(inputString);

    // Prepare the message to be sent
    // Get datetime
    DateTime now = rtc.now();
    char buf1[20];
    sprintf(buf1, "%02d%02d%02d%02d%02d%02d ",  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );

    // Remove the new line character at the end of the message string
    inputString.trim();
    // lenght of the string
    int str_len = inputString.length() + 1;
    // prepare the character array (the buffer)
    char char_array[str_len];
    // copy the message over the array
    inputString.toCharArray(char_array, str_len);

    // Prepare the character array for sending
    int lora_msg_len = 20 + str_len;
    char LoRa_message[lora_msg_len];

    strcpy(LoRa_message, buf1);
    strcat(LoRa_message, char_array);

    // send over LoRa
    //rf95.send((uint8_t *)char_array, str_len);
    rf95.send((uint8_t *)LoRa_message, lora_msg_len);

    // Wait for message to get back
    Serial.println("Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    Serial.println("Waiting for reply...");
    if (rf95.waitAvailableTimeout(1000))
    { 
      // Should be a reply message for us now   
      if (rf95.recv(buf, &len))
      {
        Serial.print("Got reply: ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
      }
      else
      {
        Serial.println("Receive failed");
      }
    }
    else
    {
      Serial.println("No reply, is there a listener around?");
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  // // Prepare a message to be sent
  // DateTime now = rtc.now();
  // char buf1[20];
  // sprintf(buf1, "%02d%02d%02d%02d%02d%02d",  now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() );
  // //myString = buf1 + myString;
  // //Serial.println(myString);
  // rf95.send((uint8_t *)buf1, 20);

  // Serial.println("Waiting for packet to complete..."); 
  // delay(10);
  // rf95.waitPacketSent();
  // // Now wait for a reply
  // uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);

  // Serial.println("Waiting for reply...");
  // if (rf95.waitAvailableTimeout(1000))
  // { 
  //   // Should be a reply message for us now   
  //   if (rf95.recv(buf, &len))
  //  {
  //     Serial.print("Got reply: ");
  //     Serial.println((char*)buf);
  //     Serial.print("RSSI: ");
  //     Serial.println(rf95.lastRssi(), DEC);    
  //   }
  //   else
  //   {
  //     Serial.println("Receive failed");
  //   }
  // }
  // else
  // {
  //   Serial.println("No reply, is there a listener around?");
  // }

}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
