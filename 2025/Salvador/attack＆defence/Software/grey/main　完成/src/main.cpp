#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <Servo.h>
#include <engelline.hpp>
#include "engelline.cpp"

#include <motor.h>
#include "vector.cpp"
#include <kicker.hpp>
#include "kicker.cpp"

#include <oled.h>
#include <linecul.h>

#define STARTPIN 3
#define role_changepin 36
#define camera_changepin 35
#define oled_changepin 34

#define front_line 9

#define TEST 32
#define TOUCH 13
#define kicker 30

//////oled関連///////////////////////////////////////////////////////////
int move_speed = 100;

int oled_start_;
bool start_sceen = true;
int oled_start_time;
bool oled_reset = false;

/// 共通 /////////////////////////////////////////////////////////////
// ボタン
int Start;
int role_change;
int camera_change;
int oled_change;

// ball
int IR_dir;
int IR_dis;

// kicker
int touch;
int kick_time;
int kick_test_time;
Kicker kicker_; // Kickerクラスのオブジェクトを作成

// cammera
int court_dir;
int yellow_dir;
int yellow_dis;
int blue_dir;
int blue_dis;
int atack_goal_dis;
int defence_goal_dis;

// time
int serial_count = 0;

/// アタッカー ////////////////////////////////////////////////////
int line_time_all;
int line_time_deep;

int goal_pid_select;
int goal_gyro_dir;
bool goal_mode;

long RandomNumber; // 「RandomNumber」をlongで変数宣言
bool random_first = true;
bool start_first = true;

int kick_off_time;
int kick_touch_time;

/// ゴールキーパー ///////////////////////////////////////////////

int line_start_;

int line_re;
bool line_research = true;
int line_bit_serial;
bool line_val = false;
int IR_line_change;
int goal_line_change;
int go_line = 500;
int goal_dir_leave = 500;

int IR_pre_dis = 0;
int IR_new_dis = 0;
int IR_dif = 0;
int IR_dif_sum = 0;
int ball_time;

int com_mode = 0;

int rulu_time = 0;
int rulu_pre_time = 0;
bool KICK_fin = true;
int mode_atack_pin;
bool atack_test = false;
bool own_goal = false;

//////////////////////////////////////////////////////////////////

void setup()
{
  analogWriteResolution(10);

  // Wire1.setSDA(17);
  // Wire1.setSCL(16);

  pinMode(STARTPIN, INPUT_PULLDOWN);
  pinMode(role_changepin, INPUT_PULLDOWN);
  pinMode(camera_changepin, INPUT_PULLDOWN);
  pinMode(oled_changepin, INPUT_PULLDOWN);

  pinMode(TEST, OUTPUT);
  pinMode(TOUCH, INPUT_PULLDOWN);
  pinMode(kicker, OUTPUT);
  pinMode(resetpin, INPUT_PULLDOWN);
  pinMode(front_line, INPUT);
  oled_setup();
  kicker_.init(32, 30); // Kickerクラスのsetupを呼ぶ

  Serial.begin(115200);

  Serial1.begin(115200); // motor
  Serial1.setTimeout(10);

  Serial2.begin(115200); // IR
  Serial2.setTimeout(10);

  Serial3.begin(115200); // camera
  Serial3.setTimeout(10);

  Serial5.begin(115200); // line
  Serial5.setTimeout(10);

  if (!bno.begin(OPERATION_MODE_IMUPLUS))
  {
    Serial.print("Ooops, no BNO055 detected ...");
    while (1)
      ;
  }
  bno.setExtCrystalUse(true);
}
void attacker_setup()
{
  if (random_first == true)
  {
    RandomNumber = random(1, 101); // 1〜2の範囲でランダム数を生成
    random_first = false;
  }

  process_engelline();
  atack_goal_dir = atack_goal_dir * -1;
  if (IR_dir < 0)
  {
    IR_dir += 360;
  }
}

void attacker()
{

  move_speed = 90;
  if (line_bit > 0 || front_line_val > LINE_FRONT)
  {
    LINE_reaction.start(); // line timer スタート
    LINE_reaction.tick();
    line_time_all = LINE_reaction.get_value();
    if (mass1 < 500)
    {
      if (is_halfout)
      {
        Move_Deg(line_evacuation_deg + 180, move_speed + 10);
      }
      else
      {
        Move_Deg(line_evacuation_deg + 180, move_speed + 10);
      }
    }
    else
    {
      if (is_halfout)
      {

        Move_Deg(side_line * 45 + 180, move_speed + 10);
      }
      else
      {
        Move_Deg(side_line * 45, move_speed + 10);
      }
    }
    // Move_Deg(court_dir, move_speed);
  }
  else
  {
    if (line_time_all < 100 && line_time_all > 0)
    {
      LINE_reaction.tick();
      line_time_all = LINE_reaction.get_value();
      if (mass1 < 500)
      {
        if (is_halfout)
        {
          Move_Deg(line_evacuation_deg + 180, move_speed + 10);
        }
        else
        {
          Move_Deg(line_evacuation_deg + 180, move_speed + 10);
        }
      }
      else
      {
        if (is_halfout)
        {

          Move_Deg(side_line * 45 + 180, move_speed + 10);
        }
        else
        {
          Move_Deg(side_line * 45, move_speed + 10);
        }
      }
    }
    else
    {
      LINE_reaction.reset();
      line_time_all = LINE_reaction.get_value();
      if (IR_dir == 500)
      {
        atack_goal_dir = -500;
        Move_Deg(0, 0);
      }
      else
      {

        if (touch == 1)
        {
          // atack_goal_dir = -500;
          if (IR_dir < 60 || IR_dir > 300)
          {
            if (atack_goal_dis < 118)
            {
              if (atack_goal_dir <= 15 && atack_goal_dir >= -15)
              {
                kicker_.kick(15, 0);
                KICK_fin = false;
              }
            }
            else
            {
              kicker_.kick(15, 150);
              KICK_fin = false;
            }
          }
          Move_Deg(0, move_speed);
        }
        else
        {
          KICK.reset();

          if (IR_dis > 11)
          {
            Move_Deg(IR_dir, move_speed);
          }
          else
          {
            if (IR_dir < 17)
            {
              Move_Deg(IR_dir, move_speed);
            }
            else if (IR_dir < 30)
            {
              Move_Deg(IR_dir + 40, move_speed);
            }
            else if (IR_dir < 60)
            {
              Move_Deg(IR_dir + 65, move_speed);
            }
            else if (IR_dir < 160)
            {
              Move_Deg(IR_dir + 60, move_speed);
            }
            else if (IR_dir < 180)
            {
              Move_Deg(IR_dir + 50, move_speed);
            }
            else if (IR_dir < 200)
            {
              Move_Deg(IR_dir - 50, move_speed);
            }
            else if (IR_dir < 300)
            {
              Move_Deg(IR_dir - 60, move_speed);
            }
            else if (IR_dir < 330)
            {
              Move_Deg(IR_dir - 65, move_speed);
            }
            else if (IR_dir < 343)
            {
              Move_Deg(IR_dir - 40, move_speed);
            }
            else
            {
              Move_Deg(IR_dir, move_speed);
            }
          }
        }
      }
    }
  }
  if (atack_goal_dir == -500)
  {
    PID_mode = 0;
  }
  else
  {
    PID_mode = 1;
  }
}

void defence_setup()
{
  if (defence_goal_dir < 500)
  {
    goal_dir_leave = defence_goal_dir;
  }
  if (defence_goal_dir >= 0)
  {
    goal_line_change = defence_goal_dir - 180;
  }
  else
  {
    goal_line_change = defence_goal_dir + 180;
  }

  // gyro();

  if (line_bit > 0)
  {
    line_val = true;
  }
  else
  {
    line_val = false;
  }
  if (mass1 == mass2)
  {
    mass2 = 500;
  }
  atack_goal_dir = atack_goal_dir * -1;
  IR_line_change = IR_dir;
  if (IR_line_change < 0)
  {
    IR_line_change += 360;
  }
}

void defence()
{

  PID_mode = 0;
  move_speed = 45;
  if (com_mode == 0)
  {

    PID_mode = 0;
    if (line_start_ == 0)
    {
      if (abs(IR_dir) > 135)
      {
        com_mode = 2;
        own_goal = true;
      }

      if (defence_goal_dir == 500)
      {
        if (line_bit > 0)
        {
          Move_Deg(court_dir, 80);
        }
        else
        {

          Move_Deg(180, 80);
        }
      }
      else
      {
        if (line_bit > 0)
        {
          if (defence_goal_dis > 110)
          {
            Move_Deg(court_dir, 80);
          }
          else
          {
            com_mode = 1;
          }
        }
        else
        {

          Move_Deg(defence_goal_dir, 80);
        }
      }
    }
  }
  else if (com_mode == 1)
  {
    PID_mode = 0;
    if (defence_goal_dir == 500)
    {
      // Move_Deg(180, 40);
      com_mode = 0;
    }
    else
    {
      // com_mode = 0
      if (defence_goal_dir > 90 && defence_goal_dis > 100)
      {
        com_mode = 0;
      }
      else if (defence_goal_dir < -90 && defence_goal_dis > 100)
      {
        com_mode = 0;
      }

      if (line_val == true)
      {

        if (mass1 < 500 && mass2 == 500 && mass3 == 500)
        {
          go_line = mass1;
          if (abs(defence_goal_dir - IR_dir) >= 45)
          {
            if (IR_dir > 0)
            {
              Move_Deg(mass1, 80);
            }
            else
            {
              Move_Deg(mass1, 80);
            }
          }
          else
          {
            Move_Deg(mass1, 20);
          }
        }
        else if (mass1 < 500 && mass2 < 500 && mass3 == 500)
        {
          rulu_time = RULU.get_value();

          if (abs(defence_goal_dir) >= 135)
          {
            if (IR_dis <= 8)
            {
              if (mass1 < 500 && mass2 < 500)
              {
                if (goal_line_change - 45 < IR_dir && goal_line_change + 45 > IR_dir)
                { //
                  RULU.start();
                  RULU.tick();
                  if (rulu_time >= 2000)
                  {
                    rulu_time = 0;
                    RULU.reset();
                    com_mode = 2;
                  }
                }
              }
              else
              {
                rulu_time = 0;
                RULU.reset();
              }
            }
            else
            {
              rulu_time = 0;
              RULU.reset();
            }
          }
          else
          {
            rulu_time = 0;
            RULU.reset();
          }

          if (mode_line == 0) ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          {
            if (IR_dir >= -180 && IR_dir < 500)
            {

              if (goal_line_change - 130 > IR_dir)
              {
                Move_Deg(mass2, 20);
              }
              else if (goal_line_change - 70 > IR_dir)
              {
                Move_Deg(mass2, 70);
              }
              else if (goal_line_change - 40 > IR_dir)
              {
                Move_Deg(mass2, 60);
              }
              else if (goal_line_change - 20 > IR_dir)
              {
                Move_Deg(mass2, 50);
              }
              else if (goal_line_change + 10 > IR_dir)
              {
                Move_Deg(0, 0);
              }
              else if (goal_line_change + 40 > IR_dir)
              {
                Move_Deg(mass1, 50);
              }
              else if (goal_line_change + 50 > IR_dir)
              {
                Move_Deg(mass1, 60);
              }
              else if (goal_line_change + 120 > IR_dir)
              {
                Move_Deg(mass1, 70);
              }
              else
              {
                Move_Deg(mass1, 20);
              }
            }
            else
            {
              // mass1 = line_dir_stop(mass1, mass2);
              if (goal_line_change > 7)
              {
                Move_Deg(mass2, move_speed + 10);
              }
              else if (goal_line_change > -7)
              {
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

            if (defence_goal_dir > 0)
            {
              if (abs(IR_dir) > 135)
              {
                Move_Deg(180, 20);
              }
              else
              {
                if (goal_line_change - 6 > IR_dir)
                {
                  if (abs(mass1 - 0) < abs(mass2 - 360))
                  {
                    Move_Deg(mass2, 20);
                  }
                  else
                  {
                    Move_Deg(mass1, 20);
                  }
                }
                else if (goal_line_change < IR_dir)
                {
                  if (abs(mass1 - 0) < abs(mass2 - 360))
                  {
                    Move_Deg(mass1, 50);
                  }
                  else
                  {
                    Move_Deg(mass2, 50);
                  }
                }
                else
                {
                  Move_Deg(0, 0);
                }
              }
            }
            else
            {
              if (abs(IR_dir) > 135)
              {
                Move_Deg(180, 20);
              }
              else
              {
                if (goal_line_change > IR_dir)
                {

                  if (abs(mass1 - 0) < abs(mass2 - 360))
                  {
                    Move_Deg(mass1, 60);
                  }
                  else
                  {
                    Move_Deg(mass2, 60);
                  }
                }
                else if (goal_line_change + 10 < IR_dir)
                {
                  if (abs(mass1 - 0) < abs(mass2 - 360))
                  {
                    Move_Deg(mass2, 20);
                  }
                  else
                  {
                    Move_Deg(mass1, 20);
                  }
                }
                else
                {
                  Move_Deg(0, 0);
                }
              }
            }
          }
          else if (mode_line == 4) //////////////////////////////////////////////////左右////////////mode4
          {
            if (IR_dir == 500)
            {
              if (goal_line_change > 7)
              {
                Move_Deg(mass2, move_speed);
              }
              else if (goal_line_change > -7)
              {
                Move_Deg(0, 0);
              }
              else
              {

                Move_Deg(mass1, move_speed);
              }
            }
            else
            {
              if (IR_dis <= 8)
              {
                if (goal_line_change - 117 > IR_dir)
                {
                  Move_Deg(270, 20);
                }
                else if (goal_line_change - 80 > IR_dir)
                {
                  Move_Deg(270, 90);
                }
                else if (goal_line_change - 40 > IR_dir)
                {
                  Move_Deg(270, 80);
                }
                else if (goal_line_change - 11 > IR_dir)
                {
                  Move_Deg(270, 60);
                }
                else if (goal_line_change + 20 > IR_dir)
                {
                  Move_Deg(0, 0);
                }
                else if (goal_line_change + 40 > IR_dir)
                {
                  Move_Deg(90, 60);
                }
                else if (goal_line_change + 90 > IR_dir)
                {
                  Move_Deg(90, 80);
                }
                else if (goal_line_change + 120 > IR_dir)
                {

                  Move_Deg(90, 90);
                }
                else
                {
                  Move_Deg(90, 20);
                }
              }
              else
              {
                if (goal_line_change - 170 > IR_dir)
                {
                  Move_Deg(270, 20);
                }
                else if (goal_line_change - 30 > IR_dir)
                {
                  Move_Deg(270, 90);
                }
                else if (goal_line_change - 25 > IR_dir)
                {
                  Move_Deg(270, 80);
                }
                else if (goal_line_change - 10 > IR_dir)
                {
                  Move_Deg(270, 50);
                }
                else if (goal_line_change > IR_dir)
                {
                  Move_Deg(0, 0);
                }
                else if (goal_line_change + 10 > IR_dir)
                {
                  Move_Deg(90, 50);
                }
                else if (goal_line_change + 25 > IR_dir)
                {
                  Move_Deg(90, 80);
                }
                else if (goal_line_change + 160 > IR_dir)
                {

                  Move_Deg(90, 90);
                }
                else
                {
                  Move_Deg(90, 20);
                }
              }
            }
          }
        }
        else if (mass1 < 500 && mass2 < 500 && mass3 < 500)
        {
          if (IR_dir == 500)
          {
            if (defence_goal_dir > 0)
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
              if (abs(mass1 - 0) < abs(mass2 - 360))
              {
                Move_Deg(mass2, move_speed);
              }
              else
              {
                Move_Deg(mass1, move_speed);
              }
            }
          }
          else
          {
            if (abs(IR_dir) > 100)
            {
              Move_Deg(0, 0);
            }
            else
            {
              if (defence_goal_dir > 0)
              {
                Move_Deg(mass1, 60);
              }
              else
              {
                Move_Deg(mass3, 60);
              }
            }
          }
        }
      }
      else
      {
        Move_Deg(go_line, 60);
      }
    }
  }
  else if (com_mode == 2)
  {
    move_speed = 80;
    if (atack_goal_dir == 500 || atack_goal_dir == -500)
    {
      PID_mode = 0;
    }
    else
    {
      PID_mode = 1;
    }
    if (atack_test == false)
    {
      RULU.start();
      RULU.tick();
    }
    rulu_time = RULU.get_value();

    if (rulu_time >= 500)
    {
      com_mode = 0;
      RULU.reset();
    }
    else if (rulu_time >= 0 && rulu_time <= 300)
    {

      Move_Deg(IR_line_change, 70);
    }
    else
    {
      if (line_val == true)
      {
        LINE.start(); // line timer スタート
        LINE.tick();
        line_time = LINE.get_value();
        Move_Deg(court_dir, 90);
      }
      else
      {
        LINE.tick();
        line_time = LINE.get_value();

        if (line_time > 0 && line_time < 200)
        {

          Move_Deg(court_dir, 90);
        }
        else
        {
          LINE.reset();
          if (IR_line_change == 500)
          {
            Move_Deg(0, 0);
          }
          else
          {
            if (touch == 1)
            {
              kicker_.kick(2000, 100);
              Move_Deg(0, move_speed);
            }
            else
            {
              if (IR_dis < 500)
              {
                if (IR_line_change < 10)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change * 1.4, move_speed);
                  }
                }
                else if (IR_line_change < 30)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change + 32, move_speed);
                  }
                }
                else if (IR_line_change < 60)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change + 70, move_speed);
                  }
                }
                else if (IR_line_change < 160)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change + 60, move_speed);
                  }
                }
                else if (IR_line_change < 180)
                {

                  Move_Deg(IR_line_change + 60, move_speed);
                }
                else if (IR_line_change < 210)
                {

                  Move_Deg(IR_line_change - 60, move_speed);
                }
                else if (IR_line_change < 300)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change - 60, move_speed);
                  }
                }
                else if (IR_line_change < 330)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change - 70, move_speed);
                  }
                }
                else if (IR_line_change < 350)
                {

                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change - 40, move_speed);
                  }
                }
                else
                {
                  IR_line_change = IR_line_change - 360;
                  if (own_goal == true)
                  {
                    com_mode = 0;
                    own_goal = false;
                  }
                  else
                  {
                    Move_Deg(IR_line_change * 1.4, move_speed);
                  }
                }
              }
              else
              {
                com_mode = 0;
              }
            }
          }
        }
      }
    }
  }
}

void loop()
{

  is_line_detected();
  // is_line_evacuation();
  is_previous_line_detected();
  kicker_.loop(); // Kickerクラスのloopを呼ぶ
  digitalWrite(TEST, HIGH);
  touch = digitalRead(TOUCH);
  Start = digitalRead(STARTPIN);
  role_change = digitalRead(role_changepin);
  camera_change = digitalRead(camera_changepin);
  oled_change = digitalRead(oled_changepin);
  front_line_val = analogRead(front_line);
  ///////////oled関係
  oled_button();
  if (digitalRead(oled_start_pin) == 1)
  {
    oled_reset = false;
    OLED.start();
    OLED.tick();
    oled_start_time = OLED.get_value();
    if (oled_start_time > 50)
    {
      Gyro_cal(gyro_deg);
      IR_dir_cal(IR_dir);
      Line_cal(line_bit, front_line_val);
      Camera_dir_cal(yellow_dir, blue_dir, court_dir);
      oled_main();
      oled_start_time = 0;
      OLED.reset();
    }
  }
  else
  {
    if (oled_reset == false)
    {
      reset_oled();
      oled_reset = true;
      screen_num = 0;
    }
  }

  //////////値の読み取り
  gyro();

  while (Serial2.available() > 0) // ir
  {
    IR_dir = Serial2.readStringUntil('a').toInt();
    IR_dis = Serial2.readStringUntil('b').toInt();
  }

  while (Serial3.available() > 0) // camera
  {
    court_dir = Serial3.readStringUntil('a').toInt();
    yellow_dir = Serial3.readStringUntil('b').toInt();
    yellow_dis = Serial3.readStringUntil('c').toInt();
    blue_dir = Serial3.readStringUntil('d').toInt();
    blue_dis = Serial3.readStringUntil('e').toInt();
  }

  while (Serial5.available() > 0) // line
  {
    line_bit = Serial5.readStringUntil('a').toInt();
  }
  mass1 = 500;
  mass2 = 500;
  mass3 = 500;
  line_main();

  defence_goal_dir = yellow_dir;
  defence_goal_dis = yellow_dis;
  atack_goal_dir = blue_dir;
  atack_goal_dis = blue_dis;

  // defence_goal_dir = blue_dir;
  // defence_goal_dis = blue_dis;
  // atack_goal_dir = yellow_dir;
  // atack_goal_dis = yellow_dis;

  if (role_change == 0)
  {
    attacker_setup();
    if (atack_goal_dir == -500)
    {
      PID_mode = 0;
    }
  }
  else
  {
    defence_setup();
  }

  if (Start == 1)
  {
    if (role_change == 0)
    {
      if (start_first == true)
      {
        KICKOFF.start();
        start_first = false;
      }
      else
      {
        KICKOFF.tick();
        kick_off_time = KICKOFF.get_value();
        if (0 < kick_off_time && kick_off_time < 50 && touch == 1)
        {
          KICKOFF.reset();
          STARTkick.start(); // line timer スタート
          STARTkick.tick();
          kick_touch_time = STARTkick.get_value();
        }
        else if (kick_off_time >= 100)
        {
          KICKOFF.reset();
          kick_off_time = KICKOFF.get_value();
        }
        else
        {
          STARTkick.tick();
          kick_touch_time = STARTkick.get_value();
          if (0 < kick_touch_time && kick_touch_time < 400)
          {
            if (RandomNumber <= 50)
            {
              Move_Deg(45, 100);
            }
            else
            {
              Move_Deg(-45, 100);
            }
          }
          else if (kick_touch_time >= 400)
          {
            if (touch == 1)
            {
              kicker_.kick(5000, 0);
              KICK_fin = false;
            }
            else
            {
              STARTkick.reset();
            }
            Move_Deg(0, move_speed);
          }
          else if (kick_touch_time == 0)
          {
            attacker();
          }
        }
      }
    }
    else
    {
      defence();
    }
  }
  else
  {
    if (mode_change == 1)
    {
      kicker_.kick(0, 100); // Kickerクラスのkickを呼ぶ
      KICK_fin = false;
      mode_change = 0;
    }

    move(0, 0, 0, 0);
  }

  Serial.print("IR_dir ");
  Serial.print(IR_dir);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("IR_dis ");
  // Serial.print(IR_dis);
  // Serial.print("   ");
  // Serial.print("\t");

  Serial.print("line_bit ");
  Serial.print(line_bit);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("gyro_dir ");
  Serial.print(gyro_deg);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("goal_gyro_dir ");
  Serial.print(goal_gyro_dir);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("court_dir ");
  // Serial.print(court_dir);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("goal_dir ");
  // Serial.print(defence_goal_dir);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("line_change ");
  // Serial.print(goal_line_change);
  // Serial.print("   ");

  // Serial.print("atack_goal_dir ");
  // Serial.print(atack_goal_dir);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("goal_dis ");
  // Serial.print(defence_goal_dis);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("front ");
  // Serial.print(front_line_val);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("camera_change ");
  // Serial.print(camera_change);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("role_change ");
  // Serial.print(role_change);
  // Serial.print("   ");
  // Serial.print("\t");
  // Serial.print("com ");
  // Serial.print(com_mode);
  // Serial.print("   ");
  // Serial.print("side ");
  // Serial.print(side_line);
  // Serial.print("   ");
  // Serial.print("evacuation_deg ");
  // Serial.print(line_evacuation_deg);
  // if (is_halfout)
  // {

  //   Serial.print("halfout");
  // }
  // Serial.print("kick_touch_time ");
  // Serial.print(kick_touch_time);
  // Serial.print("   ");
  // Serial.print("\t");
  // Serial.print("kick_off_time ");
  // Serial.print(kick_off_time);
  // Serial.print("   ");
  // Serial.print("IR_li ");
  // Serial.print(IR_line_change);
  // Serial.print("   ");
  // Serial.print("rulu_time ");
  // Serial.print(rulu_time);
  // Serial.print("   ");
  // Serial.print("\t");
  Serial.println("\t");
}