// MQTT functions

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