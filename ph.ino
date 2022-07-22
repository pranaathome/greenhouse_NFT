#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <SimpleTimer.h>

#define PH_PIN  1

SimpleTimer         timer;
Adafruit_ADS1115    ads;

float calibration_value = 21.34 - 0;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;

void setup() {   
    Wire.begin();
    Serial.begin(9600);
    ads.begin();
}

void loop() {
    // read 10 data
    for (int i=0; i<10; i++) { 
        buffer_arr[i]=ads.readADC_SingleEnded(PH_PIN);
        delay(30);
    }
    // sort
    for (int i=0;i<9;i++) {
        for (int j=i+1;j<10;j++) {
            if (buffer_arr[i]>buffer_arr[j]) {
                temp=buffer_arr[i];
                buffer_arr[i]=buffer_arr[j];
                buffer_arr[j]=temp;
            }
        }
    }
    // average 6 median data
    avgval=0;
    for (int i=2; i<8; i++)
        avgval+=buffer_arr[i];
    
    float volt = (float)ads.computeVolts(avgval/6); 
    ph_act = -5.70 * volt + calibration_value;

 Serial.print("pH Val: ");
 Serial.print(volt);
 Serial.print(",");
 Serial.println(ph_act);
 delay(1000);
}


