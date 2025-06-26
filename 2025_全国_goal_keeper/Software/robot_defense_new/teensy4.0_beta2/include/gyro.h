#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>

// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire1); // 新タイプは0x29
// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire2); // 新タイプは0x29

#define resetpin 22

int reset;

int gyro_data;
// int gyro_deg;
float raw_deg;
float reset_deg;


void gyro_L3(){
    if (Serial4.available() > 0) // gyro
    {
      gyro_data = Serial4.readStringUntil('a').toInt();
    }
  
}

// void gyro()
// {

//   reset = digitalRead(resetpin);

//   // Serial.print("reset,");
//   // Serial.print(reset);
//   // Serial.print(" ");

//   imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  

//   raw_deg = euler.x();

//   if (reset == HIGH)
//   {
//     if (raw_deg > 180)
//     {
//       reset_deg = raw_deg - 360;
//     }
//     else
//     {
//       reset_deg = raw_deg;
//     }

//     reset_deg = reset_deg;
//   }

//   gyro_data = (int)(euler.x() - reset_deg) % 360;
//   gyro_data = (gyro_data) % 360;

//   if (gyro_data > 180)
//   {
//     gyro_data = gyro_data - 360;
//   }

//   // Serial.print("raw,");
//   // Serial.print(raw_deg);
//   // Serial.print(" ");
//   Serial.print("gyro,");
//   Serial.print(gyro_data);
//   Serial.print(" ");
//   // Serial.print("gyro,");
//   // Serial.print(reset_deg);
//   // Serial.print(" ");


// }
