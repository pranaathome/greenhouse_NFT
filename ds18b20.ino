/*  16. OLED display
    https://lastminuteengineers.com/oled-display-arduino-tutorial/
*/
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 3 on the Arduino
#define ONE_WIRE_BUS D3

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

Adafruit_SSD1306 oled(-1);

void setup() {
  Serial.begin(115200);
  sensors.begin();  // Start up the library
//  oled.begin();
//  oled.clearDisplay();
//  oled.setTextColor(WHITE, BLACK);
}

void loop() {
//  float Tc = get_temp();
//  show_temp(Tc);
//  delay(5000);

  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
//  Serial.print((char)176);//shows degrees character
  Serial.println(" C");
//  oled.setCursor(0, 0);
//  oled.print("Temp: ");
//  oled.print(sensors.getTempCByIndex(0));
//  oled.print((char)176);
//  oled.print(" C");
//  oled.display();
  
  //print the temperature in Fahrenheit
//  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
//  Serial.print((char)176);//shows degrees character
//  Serial.println("F");
//  oled.setCursor(0, 0);
//  oled.print(Tc);
//  oled.print(" C");
//  oled.display();
  
  delay(500);
}

//float get_temp() {
//  float mV = analogRead(A0)/1024.0 * 3200;          // 10 mV/°C
//  return mV / 10;
//}
//
//void show_temp(float Tc) {
//  Serial.println(Tc);
//  oled.setCursor(0, 0);
//  oled.print(Tc);
//  oled.print(" C");
//  oled.display();
//}
