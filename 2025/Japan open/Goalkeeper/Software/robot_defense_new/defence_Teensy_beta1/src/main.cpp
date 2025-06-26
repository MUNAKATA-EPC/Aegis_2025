#include <Arduino.h>

#include <motor.h>
#include <line_.h>

#define oled_start_pin 2
#define STARTPIN 23
#define start_pin 3

int a;
int b;
int Start = 0;
int serial_count = 0;
int move_speed = 0;
int start = 0;
//////IR関連////////
int IR_dir;
int IR_dis;

//////oled関連/////
int oled_start_;

/////LINE閾値///////

#define smallright 67.5
#define bigright 112.5
#define smallleft 247.5
#define bigleft 292.5
int line_re;
bool line_research = true;
int line_bit_serial;
bool line_val = false;
int IR_line_change;
int goal_line_change;
int go_line = 500;
int goal_dir_leave = 500;
void setup()
{
  pinMode(oled_start_pin, INPUT_PULLDOWN);
  pinMode(STARTPIN, INPUT_PULLDOWN);
  pinMode(start_pin, INPUT_PULLDOWN);

  Serial.begin(9600);

  Serial1.begin(115200); // motor
  // Serial1.setTimeout(10);

  Serial2.begin(9600); // IR
  // Serial2.setTimeout(20);

  Serial5.begin(115200); // line
  // Serial5.setTimeout(20);

  Serial3.begin(9600); // camera
  // Serial3.setTimeout(10);

  pinMode(resetpin, INPUT_PULLDOWN);

  if (!bno.begin(OPERATION_MODE_IMUPLUS))
  {
    while (1)

      // Serial.print("Ooops, no BNO055 detected ...")
      ;
  }

  bno.setExtCrystalUse(true);
}

void loop()
{

  // line_bit = 0;
  mass_cou = 0;
  mass1 = 500;
  mass2 = 500;
  mass3 = 500;
  mode_line = 0;
  // goal_dir = 500;
  // court_dir = 500;

  serial_count += 1;

  // if (oled_start_ == 1)
  // {
  //   if (line_research)
  //   {
  //     line_re = 1;
  //     Serial5.print(String(line_re));
  //     Serial5.println(String('a'));
  //     delay(2000);
  //     line_research = false;
  //   }
  // }

  if (serial_count < 9)
  {
    while (Serial5.available() > 0) // line
    {
      line_bit = Serial5.readStringUntil('a').toInt();
    }
  }
  else if (serial_count >= 9 && serial_count < 14)
  {
    while (Serial2.available() > 0) // IR
    {
      IR_dir = Serial2.readStringUntil('a').toInt();
      IR_dis = Serial2.readStringUntil('b').toInt();
    }
  }
  else if (serial_count >= 14 && serial_count < 19)
  {
    while (Serial3.available() > 0) // camera
    {
      goal_dir = 500;
      court_dir = Serial3.readStringUntil('a').toInt();
      goal_dir = Serial3.readStringUntil('b').toInt();
      // blue_dir = Serial3.readStringUntil('c').toInt();
    }
  }
  else if (serial_count >= 19)
  {
    serial_count = 0;
  }

  /// IR_rx 7line_rx21

  if (oled_start_ == 1)
  {
    if (line_research)
    {
      line_re = 1;
      Serial5.print(String(line_re));
      Serial5.println(String('a'));
      delay(2000);
      line_research = false;
    }
  }
  // if(goal_dir > 180){
  //   goal_dir - 360;
  // }
  if (goal_dir < 500)
  {
    goal_dir_leave = goal_dir;
  }
  if (goal_dir >= 0)
  {
    goal_line_change = goal_dir - 180;
  }
  else
  {
    goal_line_change = goal_dir + 180;
  }

  gyro();
  oled_start_ = 0;

  Start = digitalRead(STARTPIN);
  Serial.print(Start);
  Serial.print(" ");
  oled_start_ = digitalRead(oled_start_pin);
  start = digitalRead(start_pin);

  if (line_bit > 0)
  {
    line_val = true;
  }
  else
  {
    line_val = false;
  }
  line_main();
  if (IR_dir >= -180 && IR_dir < 500)
  {
    if (IR_dir > 0 && IR_dir <= 180)
    {
      IR_line_change = IR_dir;
      if (IR_line_change > 90)
      {
        IR_line_change -= 180;
      }
      IR_line_change = abs(IR_line_change) / 2;
    }
    else if (IR_dir < 0)
    {
      IR_line_change = IR_dir;
      if (IR_line_change < -90)
      {
        IR_line_change += 180;
      }
      IR_line_change = abs(IR_line_change) / 2;
    }
  }

  if (Start == 1)
  {
    move_speed = 45;
    if (goal_dir == 500)
    {
      Move_Deg(90,90);
    }
    else
    {
      if (line_val == true)
      {
        if (mass1 < 500 && mass2 == 500 && mass3 == 500)
        {
          Move_Deg(mass1, move_speed);
        }
        else
        {

          if (mode_line == 0)
          {
            if (IR_dir >= -180 && IR_dir < 500)
            {

              if (goal_line_change - 110 > IR_dir)
              {
                Move_Deg(mass2, 30);
              }
              else if (goal_line_change - 70 > IR_dir)
              {
                Move_Deg(mass2, 90);
              }
              else if (goal_line_change - 40 > IR_dir)
              {
                Move_Deg(mass2, 60);
              }
              else if (goal_line_change - 20 > IR_dir)
              {
                Move_Deg(mass2, 50);
              }
              else if (goal_line_change + 7 > IR_dir)
              {
                breaking();
                Move_Deg(0, 0);
              }
              else if (goal_line_change + 30 > IR_dir)
              {
                Move_Deg(mass1, 50);
              }
              else if (goal_line_change + 50 > IR_dir)
              {
                Move_Deg(mass1, 60);
              }
              else if (goal_line_change + 70 > IR_dir)
              {
                Move_Deg(mass1, 90);
              }
              else if (goal_line_change + 110 > IR_dir)
              {
                Move_Deg(mass1, 30);
              }
            }
            else
            {
              // mass1 = line_dir_stop(mass1, mass2);
              if (goal_line_change > 5)
              {
                Move_Deg(mass2, move_speed + 10);
              }
              else if (goal_line_change > -5)
              {
                breaking();
                Move_Deg(0, 0);
              }
              else
              {
                Move_Deg(mass1, move_speed + 10);
              }
            }
          }
          else if (mode_line == 3) ////////////////////////////////////////後ろ//////////////////////mode3
          {                     

            if (goal_dir > 0)
            {
              if (goal_line_change - 10 > IR_dir)
              {
                if (abs(mass1 - 0) < abs(mass2 - 360))
                {
                  Move_Deg(mass2, 30);
                }
                else
                {
                  Move_Deg(mass1, 30);
                }
              }
              else if (goal_line_change < IR_dir)
              {
                if (abs(mass1 - 0) < abs(mass2 - 360))
                {
                  Move_Deg(mass1, move_speed);
                }
                else
                {
                  Move_Deg(mass2, move_speed);
                }
              }
              else
              {
                breaking();
                Move_Deg(0, 0);
              }
            }
            else
            {
              if (goal_line_change > IR_dir)
              {
                if (abs(mass1 - 0) < abs(mass2 - 360))
                {
                  Move_Deg(mass1, 30);
                }
                else
                {
                  Move_Deg(mass2, 30);
                }
              }
              else if (goal_line_change + 10 < IR_dir)
              {
                if (abs(mass1 - 0) < abs(mass2 - 360))
                {
                  Move_Deg(mass2, move_speed);
                }
                else
                {
                  Move_Deg(mass1, move_speed);
                }
              }
              else
              {
                breaking();
                Move_Deg(0, 0);
              }
            }
          }
          else if (mode_line == 4) //////////////////////////////////////////////////左右////////////mode4
          {
            if (mass3 == 500)
            {
              if (IR_dir == 500)
              {
                if (goal_line_change > 5)
                {
                  Move_Deg(mass2, move_speed);
                }
                else if (goal_line_change > -5)
                {
                  breaking();
                  Move_Deg(0, 0);
                }
                else
                {

                  Move_Deg(mass1, move_speed);
                }
              }
              else
              {
                if (goal_line_change - 110 > IR_dir)
                {
                  Move_Deg(270, 30);
                }
                else if (goal_line_change - 70 > IR_dir)
                {
                  Move_Deg(270, 90);
                }
                else if (goal_line_change - 60 > IR_dir)
                {
                  // if (IR_dis < 18)
                  // {
                  Move_Deg(270, 60);
                  // }
                  // else
                  // {
                  //   Move_Deg(270, 50);
                  // }
                }
                else if (goal_line_change - 30 > IR_dir)
                {
                  // if (IR_dis < 18)
                  // {
                  Move_Deg(270, 50);
                  // }
                  // else
                  // {
                  //   Move_Deg(270, 40);
                  // }
                }
                else if (goal_line_change > IR_dir)
                {
                  breaking();
                  Move_Deg(0, 0);
                }
                else if (goal_line_change + 30 > IR_dir)
                {
                  // if (IR_dis < 18)
                  // {
                  Move_Deg(90, 50);
                  // }
                  // else
                  // {
                  //   Move_Deg(90, 60);
                  // }
                }
                else if (goal_line_change + 60 < IR_dir)
                {
                  // if (IR_dis < 18)
                  // {
                  Move_Deg(90, 60);
                  // }
                  // else
                  // {
                  //   Move_Deg(90, 50);
                  // }
                }
                else if (goal_line_change + 60 > IR_dir)
                {

                  Move_Deg(90, 90);
                }
                else if (goal_line_change + 110 > IR_dir)
                {
                  Move_Deg(90, 30);
                }
              }
            }
            else
            {
              if (goal_dir > 0)
              {
                Move_Deg(mass1, move_speed + 20);
              }
              else
              {
                Move_Deg(mass3, move_speed + 20);
              }
            }
          }
          // else if (mode_line == 5) ////////////////////////////////////////////エンジェルのみ//////////////mode5
          // {

          //   if (goal_line_change > IR_dir)
          //   {
          //     if (goal_dir > 0)
          //     {
          //       breaking();
          //       Move_Deg(0, 0);
          //     }
          //     else
          //     {
          //       if (abs(mass1 - 0) > abs(mass2 - 360))
          //       {
          //         if (mass2 < 500)
          //         {
          //           // go_line = mass2;
          //         }
          //         Move_Deg(mass2, move_speed);
          //       }
          //       else
          //       {
          //         if (mass1 < 500)
          //         {
          //           // go_line = mass1;
          //         }
          //         Move_Deg(mass1, move_speed);
          //       }
          //     }
          //   }
          //   else if (goal_line_change < IR_dir)
          //   {

          //     if (goal_dir > 0)
          //     {
          //       if (abs(mass1 - 0) > abs(mass2 - 360))
          //       {
          //         if (mass2 < 500)
          //         {
          //           // go_line = mass2;
          //         }
          //         Move_Deg(mass2, move_speed);
          //       }
          //       else
          //       {
          //         if (mass1 < 500)
          //         {
          //           // go_line = mass1;
          //         }
          //         Move_Deg(mass1, move_speed);
          //       }
          //     }
          //     else
          //     {
          //       breaking();
          //       Move_Deg(0, 0);
          //     }
          //   }
          // }
        }
      }
      else
      {
        Move_Deg(0, 0);
      }
    }

    // if (start == 1)
    // {

    //     Move_Deg(0,80);
      
    // }
  }
  else
  {
    // if (mode_change == 1 && KICK_fin)
    // {
    //   KICK_TEST.start();
    //   KICK_TEST.tick();
    //   kick_test_time = KICK_TEST.get_value();
    //   if (kick_test_time >= 500 && kick_test_time <= 600)
    //   {
    //     digitalWrite(TEST, LOW);
    //     digitalWrite(kicker, HIGH);
    //     delay(30);
    //     digitalWrite(kicker, LOW);
    //     KICK_TEST.reset();
    //     kick_test_time = KICK_TEST.get_value();
    //     KICK_fin = false;
    //   }
    // }
    // else if (mode_change == 0 && KICK_fin == false)
    // {
    //   KICK_fin = true;
    // }

    // goal_dir = 300;
    move(0, 0, 0, 0);
  }
  Serial.print("IR_dis,");
  Serial.print(IR_dis);
  Serial.print(" ");

  Serial.print("IR_dir,");
  Serial.print(IR_dir);
  Serial.print(" ");
  Serial.print("goal,");
  Serial.print(goal_dir);
  Serial.print(" ");
  Serial.print("linechan,");
  Serial.print(goal_line_change);
  Serial.print(" ");

  Serial.print("goline,");
  Serial.print(go_line);

  // Serial.print("a,");
  // Serial.print(a);
  // Serial.print(" ");
  // Serial.print("b,");
  // Serial.print(b);
  // Serial.print(" ");
  // Serial.print("IR_dir,");
  // Serial.print(IR_dir);
  // Serial.print(" ");

  // Serial.print("IR_dis,");
  // Serial.print(IR_dis);
  Serial.println(" ");
}
