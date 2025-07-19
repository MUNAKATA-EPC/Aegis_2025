#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>

// #include <timer.h>
#include<average.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); // 新タイプは0x29

#define resetpin 2

int reset;

float raw_deg;
float reset_deg;
int gyro_deg;

int gyro()
{
  
  reset = digitalRead(resetpin);
  // Serial.print("reset");
  // Serial.print(reset);

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  raw_deg = euler.x();

  if (reset == HIGH)
  {
    if (raw_deg > 180)
    {
      reset_deg = raw_deg - 360;
    }
    else
    {
      reset_deg = raw_deg;
    }

    reset_deg = reset_deg;
  }

  gyro_deg = (int)(euler.x() - reset_deg) % 360;
  gyro_deg = (gyro_deg) % 360;
  //   Serial.print(gyro_deg);
  //   Serial.print("\t");

  if (gyro_deg > 180)
  {
    gyro_deg = gyro_deg - 360;
  }

  return gyro_deg;
}