/***************************

Cara Mengakses dan Pemrograman Ambient Light Sensor BH1750 Menggunakan Arduino Uno

Oleh : Arduino Indonesia
Website : www.arduinoindonesia.id
Toko Online : www.workshopelectronics3in1.com
Blog : www.edukasielektronika.com

Copyright @2021

****************************/

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop()
{
  float LUX = lightMeter.readLightLevel();
  Serial.print("Cahaya: ");
  Serial.print(LUX);
  Serial.println(" LUX");
  delay(1000);
}
