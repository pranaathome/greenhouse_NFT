#include <WEMOS_SHT3X.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//WiFi
#define SSID  "Ilkomerz_GreenHouse"
#define PASS  "ilkomgr33n1P8"
#define PERIOD 60                // pH TDS read period (minutes)

// SHT30
SHT3X sht30(0x45);

// Waterflow
volatile double waterFlow = 0;

// Time of Flight
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int lox_cm = 0;

// Light Sensor
BH1750 lightMeter;
float LUX = 0;

// TDS Meter
float tds_val = 0;
float wtemp = 0;
float ph_act = 0;

OneWire           oneWire(D3);
DallasTemperature dallas(&oneWire);
Adafruit_ADS1115  ads;

// OLED
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

int count = PERIOD - 1;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  ads.begin();  
  
  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // Waterflow
  attachInterrupt(digitalPinToInterrupt(14), pulse, RISING);

  // Time of Flight  
//  if (!lox.begin()) {
//    Serial.println(F("Failed to boot VL53L0X"));
//    while(1);
//  }

  // Light Sensor
  lightMeter.begin();

  // temp sensor
  dallas.begin();

//  pinMode(D6, OUTPUT);
//  pinMode(D7, OUTPUT);

  // WiFi
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);  
  
}

void loop() {

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE, BLACK);

  // SHT30
  dallas.requestTemperatures();
  wtemp = dallas.getTempCByIndex(0);
  if(sht30.get()==0) {
    display.print(round(sht30.cTemp), 0);
    display.print("/");
    display.print(wtemp, 0);
    display.print("C,");
    display.print(round(sht30.humidity), 0);
    display.println("%");
  } else {
    display.println("Error!");
  }

  // Light Sensor
  LUX = lightMeter.readLightLevel();
  display.print(round(LUX), 0);
  display.println(" Lux");

  // Waterflow
  display.print(waterFlow); 
  display.println(" L/min");

  // Time of Flight
//  VL53L0X_RangingMeasurementData_t measure;
//  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    lox_cm = measure.RangeMilliMeter / 10;
//    display.print(lox_cm);
//    display.println(" cm"); 
//  } 
//  else {
//    display.println("-- cm");
//  }

//  if (count == PERIOD-1) {
//    // turn on TDS/pH sensor
//    digitalWrite(D6, HIGH);
//    digitalWrite(D7, HIGH);
//  }
      
  if (count == PERIOD) {

      // TDS Meter and Temp Sensor
      int   adc0  = ads.readADC_SingleEnded(0);
      float volt0 = ads.computeVolts(adc0);
      
      // temperature compensation
      float C = 1.0 + 0.02*(wtemp - 25.0);
      float V = volt0/C;  
      tds_val = (133.42*V*V*V- 255.86*V*V + 857.39*V) * 0.5;

      // pH Sensor
      int buffer_arr[9], temp; 
      for(int i=0;i<9;i++){ 
        buffer_arr[i]=ads.readADC_SingleEnded(1);
        delay(30);
      }     
      for(int i=0;i<8;i++){
        for(int j=i+1;j<9;j++){
          if(buffer_arr[i]>buffer_arr[j]){
           temp=buffer_arr[i];
           buffer_arr[i]=buffer_arr[j];
           buffer_arr[j]=temp;
           }
         }
       }
        
       float volt = (float)ads.computeVolts(buffer_arr[4]); 
      ph_act = -5.70 * volt + 21.34;
    
      count = 0;
      // turn off TDS/pH sensor
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
    }

  // TDS Meter
  display.print("TDS: ");
  display.println(tds_val, 0);

  // pH Sensor
  display.print("pH: ");
  display.println(ph_act,1);
      
  display.display();

  //WiFi
  if (WiFi.status() == WL_CONNECTED) {
    send_data();
  }
  
  count++;
  waterFlow = 0;

  delay(60000);
}

// Measure the quantity of square wave (Waterflow)
ICACHE_RAM_ATTR void pulse()   
{
  waterFlow += 1.0 / 5880.0;  
}

int getMedianNum(int bArray[], int iFilterLen) 
{
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
    bTab[i] = bArray[i];
  
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++){
    for (i = 0; i < iFilterLen - j - 1; i++){
      if (bTab[i] > bTab[i + 1]){
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

void send_data() {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, "http://api.thingspeak.com/update");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int resp_code;
  if (count != PERIOD) 
    resp_code = http.POST("api_key=2PY2Q4MB5IMILCF2&field1=" + String(sht30.cTemp) + "&field2=" + sht30.humidity + "&field3=" + LUX + "&field4=" + waterFlow + "&field5=" + lox_cm + "&field7=" + wtemp);
  else 
    resp_code = http.POST("api_key=2PY2Q4MB5IMILCF2&field1=" + String(sht30.cTemp) + "&field2=" + sht30.humidity + "&field3=" + LUX + "&field4=" + waterFlow + "&field5=" + lox_cm + "&field6=" + tds_val + "&field7=" + wtemp + "&field8=" + ph_act);
  http.end();
  Serial.print("[HTTP] "); 
  Serial.println(resp_code);
  if (resp_code == 302) {
    http.begin(client, "http://1.1.1.3/ac_portal/login.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST("opr=pwdLogin&userName=krs002&pwd=krs2015");
    http.end();
  }
}
