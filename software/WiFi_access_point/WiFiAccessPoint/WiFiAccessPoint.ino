/* Create a WiFi access point and provide a web server on it. */
// Objective is for the USER to configure the ESP to connect to Internet using local WiFi Gateway/Router
// 1: Create AP for accepting connection (done)
// 2: Upload WiFi SSID and PWD and save in EEPROM
// 3: Reboot to connect to WiFi

// Libraries for create the AP
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

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

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

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
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
