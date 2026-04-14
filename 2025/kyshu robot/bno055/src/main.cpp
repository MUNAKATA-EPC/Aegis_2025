#include <Arduino.h>
// #include <Wire.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>



#include<acceleration.h>//gyroも入ってる







void setup()
{
  // Wire.begin();
  // Wire.setClock(40000);

  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(resetpin, INPUT_PULLDOWN);

  if (!bno.begin(OPERATION_MODE_IMUPLUS))
  {
    Serial.print("Ooops, no BNO055 detected ...");
    while (1);
  }
  bno.setExtCrystalUse(true);
}

void loop()
{
  
  gyro();
  acceleration();
  // gyro_deg = 0;
  


  Serial1.print(String(gyro_deg));
  Serial1.print(String("a"));
  Serial1.println(String("\t"));
  Serial1.print(String(v));
  Serial1.print(String("b"));
  Serial1.println(String("\t"));
  Serial1.flush();

  // Serial.print(reset_deg);
  // Serial.print("\t");
  // Serial.print(gyro_deg);
  // Serial.println("\t");
  // Serial.print(raw_deg);
  // Serial.print("\t");
  // Serial.print(v);
  // Serial.println("\t");

  // Serial.print(a_x);
  // Serial.print("\t");
  // Serial.print(a_y);
  // Serial.print("\t");
  // Serial.println(v);
  
  
  


  //delay(100);

}
