#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads1115;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Hello!");
  
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 4.096V  1 bit = 2mV");
  ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  //ads1115.begin(0x48); // reading the two ozone sensors
  ads1115.begin(0x49); // reading the CO2 sensors
}

void loop(void)
{
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads1115.readADC_SingleEnded(0);
  adc1 = ads1115.readADC_SingleEnded(1);
  adc2 = ads1115.readADC_SingleEnded(2);
  adc3 = ads1115.readADC_SingleEnded(3);
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.print("AIN1: "); Serial.println(adc1);
  Serial.print("AIN2: "); Serial.println(adc2);
  Serial.print("AIN3: "); Serial.println(adc3);
  Serial.println(" ");
  
  delay(1000);
}