/* Create a WiFi access point and provide a web server on it. */
// Objective is for the USER to configure the ESP to connect to Internet using local WiFi Gateway/Router
// 1: Create AP for accepting connection (done)
// 2: Upload WiFi SSID and PWD and save in EEPROM
// 3: Reboot to connect to WiFi

// Libraries for create the AP
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
// #include <ESP8266WebServer.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Libraries from https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>

// Libraries for creating EEPROM where to save SSID and PWD of the Gateway/Router
#include "EEPROM.h"

#define EEPROM_FLASH_MARKER 0xbad00dad
typedef struct {
  
  int flashMarker;    // a unique indentifier that tells us we have written to flash
  char wifiSSID[32];  // WiFi SSID to connect to
  char wifiPW[16];    // WiFi password
  char mqttBrokerUrl[128];   // MQTT Broker URL
  char mqttAPIUrl[128];     // MQTT API URL - for use when Broker is not available directly from WiFi network
} EEPROM_DATA_T;

#ifndef APSSID
#define APSSID "ESP_Pioneer_AP"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

const char* PARAM_INPUT_1 = "SSID";
const char* PARAM_INPUT_2 = "PASSWORD";
const char* PARAM_INPUT_3 = "INPUT_X";

//ESP8266WebServer server(80);
AsyncWebServer server(80);

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    SSID: <input type="text" name="SSID">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    PASSWORD: <input type="text" name="PASSWORD">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    INPUT_X: <input type="text" name="INPUT_X">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  //WiFi.softAP(ssid, password);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });

  server.onNotFound(notFound);

  server.begin();
  Serial.println("HTTP server started");

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void loop() {
  
}
