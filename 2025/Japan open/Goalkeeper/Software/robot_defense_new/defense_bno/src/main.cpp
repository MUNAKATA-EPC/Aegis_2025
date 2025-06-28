#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>

#include <gyro.h>

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(resetpin,INPUT_PULLDOWN);

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

  Serial1.print(String(gyro_deg));
  Serial1.print(String('a'));
  Serial1.flush();

  // Serial.print(gyro_deg);
  // Serial.println(" ");

}