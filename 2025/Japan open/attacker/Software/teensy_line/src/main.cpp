#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include "motor.h"
#include "vector.cpp"

#define STARTPIN 23

float ball_deg;
float ball_dis;
int Start;
int move_speed;
void setup()
{

  pinMode(STARTPIN, INPUT_PULLDOWN);
  pinMode(resetpin, INPUT_PULLDOWN);

  Serial.begin(115200);

  Serial1.begin(115200); // motor
  Serial1.setTimeout(10);

  Serial2.begin(115200); // IR
  Serial2.setTimeout(10);



  if (!bno.begin(OPERATION_MODE_IMUPLUS))
  {
    Serial.print("Ooops, no BNO055 detected ...");
    while (1)
      ;
  }
  bno.setExtCrystalUse(true);
}

void loop()
{
  move_speed = 100;

  Start = digitalRead(STARTPIN);

  gyro();
  acceleration();
  if (v < 30 && v > -1)
  {
    v = 0;
  }

  if (Serial2.available() > 0) // ir
  {
    ball_deg = Serial2.readStringUntil('a').toFloat();
    ball_dis = Serial2.readStringUntil('b').toInt();
  }



  if (Start == 1)
  {   
    if(ball_dis == 500)
    {
      Move_Deg(0,0);

    }
    else
    {            
      if (ball_deg < 22) // 1.61
      {
        goal_dir = 300;
        Move_Deg(ball_deg * 1.4, 100);
      }

      else if (ball_deg < 338)
      {
        //ベクトルを用いて処理
        Vector vec_to_ball(radians(ball_deg), ball_dis <= 15 ? -1 : 1);//式1 ? 式2 : 式3        >式1が真→式2を実行    >式1が偽→式3を実行
        Vector vec_to_tan_ball(radians(ball_deg <= 180 ? (ball_deg + 90) : (ball_deg - 90)), max(35 - min(ball_dis, 35), 0));
    
        vec_to_ball.add(vec_to_tan_ball);
    
        Move_Deg(vec_to_ball.get_deg(), move_speed);  
      }
      else
      {
        goal_dir = 300;
        ball_deg = ball_deg - 360;
        Move_Deg(ball_deg * 1.4, 100);
      }

    } 
  }
  else
  {
    move(0, 0, 0, 0);
  }


  Serial.print("start ");
  Serial.print(Start);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("IR_dir ");
  // Serial.print(ball_deg);
  // Serial.print("   ");
  // Serial.print("\t");

  Serial.print("IR_dis ");
  Serial.print(ball_dis);
  Serial.print("   ");
  Serial.println("\t");


}
