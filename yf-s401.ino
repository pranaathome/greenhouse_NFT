/***************************************************
 This example reads Water flow sensor Sensor.

 Created 2016-3-13
 By berinie Chen <bernie.chen@dfrobot.com>

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here  https://www.dfrobot.com/wiki/index.php?title=Water_Flow_Sensor_-_1/8%22_SKU:_SEN0216
 2.This code is tested on Arduino Uno.
 ****************************************************/
volatile double waterFlow;
int i = 0;
void setup() {
  Serial.begin(9600);  //baudrate
  waterFlow = 0;
  attachInterrupt(digitalPinToInterrupt(14), pulse, RISING);

}
void loop() {
//  Serial.print("waterFlow:");
//  Serial.print(waterFlow, 5);
//  Serial.println("   L");
  delay(60000);
  //i++;
  //if (i == 120) { 
  Serial.print(waterFlow); Serial.println(" L/min"); i = 0; waterFlow = 0; 
  //}
}

ICACHE_RAM_ATTR void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 5880.0;  
}
