#include <DallasTemperature.h>
#include <Adafruit_ADS1X15.h>

#define TDS_PIN     0
#define WTEMP_PIN   D3

OneWire           oneWire(WTEMP_PIN);
DallasTemperature dallas(&oneWire);
Adafruit_ADS1115  ads; 

void setup()
{
    Serial.begin(115200);
    dallas.begin();
    // ads.setGain(GAIN_TWOTHIRDS);
    ads.begin();
}

void loop()
{
    int   adc0  = ads.readADC_SingleEnded(TDS_PIN);
    float volt0 = ads.computeVolts(adc0);
    dallas.requestTemperatures();
    float wtemp = dallas.getTempCByIndex(0);

    // temperature compensation
    float C = 1.0 + 0.02*(wtemp - 25.0);
    float V = volt0/C;  
    float tds_val = (133.42*V*V*V- 255.86*V*V + 857.39*V) * 0.5;

    // kalibrasi
    tds_val = 1.01*tds_val - 60.4;

    Serial.print(tds_val, 0);
    Serial.print(",");
    Serial.println(wtemp);
}
