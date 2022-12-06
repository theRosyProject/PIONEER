// Loop functions

void loop()
{
  // start the mqtt loop
  client.loop();

  // save the current time into a variable
  //DateTime alive_now = rtc.now();

  // debug to send serial message over the MQTT
  // if (Serial.available() > 0) {
  //   String myString = Serial.readString();

  //   DateTime now = rtc.now();
  //   char buf1[20];
  //   sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d: ",  now.hour(), now.minute(), now.second(), now.month(), now.day(), now.year());
  //   myString = buf1 + myString;
  //   Serial.println(myString);
  //   //client.publish("test", myString);
  //   client.publish(TOPIC, myString);
  // } else {
  //   
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