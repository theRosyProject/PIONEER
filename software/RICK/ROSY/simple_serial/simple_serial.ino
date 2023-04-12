void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(50);
  }
  Serial.println("");
  Serial.println("Serial Started");
}

void loop() {
  //Serial.println("Hello World!")
  Serial.write("Hello World!");
  delay(1000);
}