#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <Servo.h>

#include <motor.h>
#include "vector.cpp"
#include <oled.h>

Servo dribbler;

#define TEST 5
#define TOUCH 13
#define kicker 6
#define STARTPIN 23
#define camera_changepin 2
#define mode_changepin 3

//////oled関連/////
int oled_start_;
bool start_sceen = true;
int oled_start_time;
bool oled_reset = false;

/// プログラム///
int Start;
int kick_change;
int dribbler_change;

int IR_dir;
int IR_dis;
int before_ball;

int line_data;
int line_time_all;
int line_time_deep;

int touch;
int kick_time;
int kick_test_time;
int kick_camera_time;

int court_dir;
int yellow_dir;
int goal_dis;

int move_dir;
int court_IR_dir;
int goal_gyro_dir;
int goal_pid_select;

int dribbler_time;
int dribbler_gyro;

int kick_off_time;
int kick_touch_time;
int line_time_reaction;

bool KICK_fin = true;
bool goal_mode = true;

int move_speed;

long RandomNumber; // 「RandomNumber」をlongで変数宣言
bool random_first = true;
bool start_first = true;

void select_pid()
{
  if (goal_dir == 300)
  {
    goal_pid_select = 0;
    move_speed = 90;
    goal_mode = true;
  }
  else
  {
    if (-10 <= goal_gyro_dir && goal_gyro_dir <= 10) // マイナス４０、４２は中立点
    {
      goal_mode = true;
      goal_pid_select = 0;
      move_speed = 90;
    }
    else if (-10 < goal_gyro_dir && goal_gyro_dir < -25 && goal_mode == true)
    {
      goal_mode = true;
      goal_pid_select = 0;
      move_speed = 90;
    }
    else if (-10 < goal_gyro_dir && goal_gyro_dir < -25 && goal_mode == false)
    {
      goal_mode = false;
      goal_pid_select = 1;
      move_speed = 65;
    }
    else if (goal_gyro_dir <= -25)
    {
      goal_mode = false;
      goal_pid_select = 1;
      move_speed = 65;
    }
    else if (10 < goal_gyro_dir && goal_gyro_dir < 25 && goal_mode == true)
    {
      goal_mode = true;
      goal_pid_select = 0;
      move_speed = 90;
    }
    else if (10 < goal_gyro_dir && goal_gyro_dir < 25 && goal_mode == false)
    {
      goal_mode = false;
      goal_pid_select = 1;
      move_speed = 65;
    }
    else if (goal_gyro_dir >= 25)
    {
      goal_mode = false;
      goal_pid_select = 1;
      move_speed = 65;
    }
  }
}
void line_back_vector()
{
  //   Vector vec_to_ball(radians(IR_dir), 200);
  //   Vector vec_to_court(radians(court_dir), 150);

  //   vec_to_ball.add(vec_to_court);

  //   if (-45 <= court_dir && court_dir <= 45)
  //   {
  //     Move_Deg(vec_to_ball.get_deg(), 100);
  //   }
  //   else
  // {
  Move_Deg(court_dir, 100);
  // }
}

void line_reaction_hamaruna()
{
  if (court_dir < 0)
  {
    court_dir = 100;
  }
  else if (court_dir > 0)
  {
    court_dir = -100;
  }
  else
  {
    court_dir = court_dir;
  }
  Move_Deg(court_dir, 100);
}

void setup()
{
  analogWriteResolution(10);

  // Wire1.setSDA(17);
  // Wire1.setSCL(16);

  pinMode(STARTPIN, INPUT_PULLDOWN);
  pinMode(camera_changepin, INPUT_PULLDOWN);
  pinMode(mode_changepin, INPUT_PULLDOWN);
  pinMode(TEST, OUTPUT);
  pinMode(TOUCH, INPUT_PULLDOWN);
  pinMode(kicker, OUTPUT);
  // pinMode(resetpin, INPUT_PULLDOWN);
  oled_setup();

  Serial.begin(115200);

  Serial1.begin(115200); // motor
  Serial1.setTimeout(10);

  Serial2.begin(115200); // IR
  Serial2.setTimeout(10);

  Serial3.begin(115200); // camera
  Serial3.setTimeout(10);

  Serial4.begin(115200); // gyro
  // Serial4.setTimeout(10);

  Serial5.begin(115200); // line
  Serial5.setTimeout(10);

  // if (!bno.begin(OPERATION_MODE_IMUPLUS))
  // {
  //   Serial.print("Ooops, no BNO055 detected ...");
  //   while (1)
  //     ;
  // }
  // bno.setExtCrystalUse(true);
}

void loop()
{
  digitalWrite(TEST, HIGH);
  touch = digitalRead(TOUCH);
  Start = digitalRead(STARTPIN);
  oled_start_ = digitalRead(camera_changepin);
  kick_change = digitalRead(mode_changepin);
  oled_button();

  // gyro();

  while (Serial2.available() > 0) // ir
  {
    IR_dir = Serial2.readStringUntil('a').toInt();
    IR_dis = Serial2.readStringUntil('b').toInt();
  }

  if (Serial3.available() > 0) // camera
  {
    court_dir = Serial3.readStringUntil('a').toInt();
    goal_dir = Serial3.readStringUntil('b').toInt();
    goal_dis = Serial3.readStringUntil('c').toInt();
  }

  if (Serial4.available() > 0) // gyro
  {
    gyro_deg = Serial4.readStringUntil('a').toInt();
  }

  goal_gyro_dir = goal_dir - gyro_deg * 2.375;

  while (Serial5.available() > 0) // line
  {
    line_data = Serial5.readStringUntil('a').toInt();
  }
  if (oled_start_ == 1)
  {
    oled_reset = false;
    if (start_sceen == true)
    {

      oled_aegis();
      start_sceen = false;
    }
    else if (start_sceen == false)
    {
      OLED.start();
      OLED.tick();
      oled_start_time = OLED.get_value();
      if (oled_start_time > 50)
      {
        IR_dir_cal(IR_dir);
        Gyro_cal(gyro_deg);
        Line_cal(line_data);
        Camera_dir_cal(goal_dir, goal_dis, court_dir);
        oled_main();
        oled_start_time = 0;
        OLED.reset();
      }
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
  // Serial.print("oled");
  // Serial.print(oled_start_);

  if (random_first == true)
  {
    RandomNumber = random(1, 101); // 1〜2の範囲でランダム数を生成
    random_first = false;
  }

  if (Start == 1)
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
        else if (kick_touch_time >= 500)
        {
          if (touch == 1)
          {
            digitalWrite(TEST, LOW);
            digitalWrite(kicker, HIGH);
            delay(40);
            digitalWrite(kicker, LOW);
            delay(100);
            KICK.reset();
          }
          else
          {
            STARTkick.reset();
          }
        }
        else if (kick_touch_time == 0)
        {
          if (line_data == 1)
          {
            select_pid();
            line_back_vector();
            LINE_reaction.start(); // line timer スタート
            LINE_reaction.tick();
            line_time_reaction = LINE_reaction.get_value();
          }
          else if (line_data == 2)
          {
            select_pid();
            LINE_deep.start(); // line timer スタート
            LINE_deep.tick();
            line_time_deep = LINE_deep.get_value();
            LINE_reaction.tick();
            line_time_reaction = LINE_reaction.get_value();
            line_back_vector();
          }
          else
          {
            LINE_deep.tick();
            line_time_deep = LINE_deep.get_value();
            LINE_reaction.tick();
            line_time_reaction = LINE_reaction.get_value();
            if (line_time_reaction >= 1000)
            {
              select_pid();
              line_reaction_hamaruna();
              LINE_reaction.reset();
            }
            else
            {
              LINE_reaction.reset();
              if (line_time_deep > 0 && line_time_deep < 150)
              {
                select_pid();
                line_back_vector();
              }
              else
              {
                LINE_deep.reset();

                if (IR_dis == 500)
                {
                  goal_pid_select = 0;
                  Move_Deg(0, 0);
                }
                else
                {
                  kick_time = KICK.get_value();
                  if (touch == 1)
                  {
                    if (IR_dir < 20 || IR_dir > 340)
                    {
                      goal_pid_select = 1;
                      KICK.start();
                      KICK.tick();
                      if (goal_dis < 120)
                      {
                        if (goal_dir <= 15 && goal_dir >= -15)
                        {
                          goal_pid_select = 1;
                          digitalWrite(TEST, LOW);
                          digitalWrite(kicker, HIGH);
                          delay(40);
                          digitalWrite(kicker, LOW);
                          delay(100);
                          KICK.reset();
                        }
                      }
                      else
                      {
                        if (kick_time >= 250) // 175
                        {
                          goal_pid_select = 1;
                          digitalWrite(TEST, LOW);
                          digitalWrite(kicker, HIGH);
                          delay(40);
                          digitalWrite(kicker, LOW);
                          KICK.reset();
                          kick_time = 0;
                        }
                      }
                      Move_Deg(0, 90);
                    }
                  }
                  else
                  {
                    // KICK.tick();
                    KICK.reset();
                    if (IR_dis <= 14)
                    {
                      if (IR_dir < 15) // 1.61
                      {
                        select_pid();
                        Move_Deg(0, move_speed);
                      }
                      else if (IR_dir < 30)
                      {
                        select_pid();
                        if (goal_pid_select == 0)
                        {
                          Move_Deg(IR_dir + 15, move_speed );
                        }
                        else if (goal_pid_select == 1)
                        {
                          Move_Deg(IR_dir + 50, move_speed);
                        }
                      }
                      else if (IR_dir < 60)
                      {
                        select_pid();
                        if (goal_pid_select == 0)
                        {
                          Move_Deg(IR_dir + 40, move_speed - 10);
                        }
                        else if (goal_pid_select == 1)
                        {
                          Move_Deg(IR_dir + 60, move_speed);
                        }
                      }
                      else if (IR_dir < 160)
                      {
                        goal_pid_select = 0;
                        move_speed = 90;
                        Move_Deg(IR_dir + 60, move_speed);
                      }
                      else if (IR_dir < 190)
                      {
                        goal_pid_select = 0;
                        move_speed = 90;
                        Move_Deg(IR_dir + 40, move_speed);
                      }
                      else if (IR_dir < 210)
                      {
                        goal_pid_select = 0;
                        move_speed = 90;
                        Move_Deg(IR_dir - 40, move_speed);
                      }
                      else if (IR_dir < 300)
                      {
                        goal_pid_select = 0;
                        move_speed = 90;
                        Move_Deg(IR_dir - 60, move_speed - 10);
                      }
                      else if (IR_dir < 330)
                      {
                        select_pid();
                        if (goal_pid_select == 0)
                        {
                          Move_Deg(IR_dir - 40, move_speed );
                        }
                        else if (goal_pid_select == 1)
                        {
                          Move_Deg(IR_dir - 60, move_speed);
                        }
                      }
                      else if (IR_dir < 345)
                      {
                        select_pid();
                        if (goal_pid_select == 0)
                        {
                          Move_Deg(IR_dir - 15, move_speed);
                        }
                        else if (goal_pid_select == 1)
                        {
                          Move_Deg(IR_dir - 50, move_speed);
                        }
                      }
                      else
                      {
                        select_pid();
                        IR_dir = IR_dir - 360;
                        Move_Deg(0, move_speed); // 1.6
                      }
                    }
                    else
                    {
                      goal_pid_select = 0;
                      Move_Deg(IR_dir, 100);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    Serial.print(line_time_deep);
    Serial.print("\t");
    Serial.print(line_time_reaction);
    Serial.print("\t");
  }
  else
  {
    random_first = true;
    move(0, 0, 0, 0);
    // if (kick_change == 1 && KICK_fin)
    // {
    //   digitalWrite(TEST, LOW);
    //   digitalWrite(kicker, HIGH);
    //   delay(30);
    //   digitalWrite(kicker, LOW);

    //   KICK_fin = false;
    // }
    // else if (kick_change == 0 && KICK_fin == false)
    // {
    //   KICK_fin = true;
    // }
    if (mode_change == 1)
    {
      digitalWrite(TEST, LOW);
      digitalWrite(kicker, HIGH);
      delay(30);
      digitalWrite(kicker, LOW);
      mode_change = 0;
    }
  }

  if (goal_dir == 300)
  {
    goal_pid_select = 0;
  }

  if (goal_pid_select == 0)
  {
    PID_mode = 0;
  }
  else
  {
    PID_mode = 1;
  }

  ////////////////スイッチ////////////////////////////
  // Serial.print("start ");
  // Serial.print(Start);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("modechange ");
  // Serial.print(mode_change);
  // Serial.print("   ");
  // Serial.print("\t");

  //////////////カメラ//////////////

  Serial.print("goal_gyro_dir ");
  Serial.print(goal_gyro_dir);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("goal_dis ");
  Serial.print(goal_dis);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("goal_dir ");
  Serial.print(goal_dir);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("pid_select ");
  // Serial.print(goal_pid_select);
  // Serial.print("   ");
  // Serial.print("\t");

  Serial.print("court_dir ");
  Serial.print(court_dir);
  Serial.print("   ");
  Serial.print("\t");
  /////////////////IR////////////////
  Serial.print("IR_dir ");
  Serial.print(IR_dir);
  Serial.print("   ");
  Serial.print("\t");

  Serial.print("IR_dis ");
  Serial.print(IR_dis);
  Serial.print("   ");
  Serial.print("\t");

  /////////////////ライン///////////////

  Serial.print("line_data ");
  Serial.print(line_data);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("line_time_all ");
  // Serial.print(line_time_all);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("line_time_deep ");
  // Serial.print(line_time_deep);
  // Serial.print("   ");
  // Serial.print("\t");

  //////////////////その他/////////////////
  Serial.print("touch ");
  Serial.print(touch);
  Serial.print("   ");
  Serial.print("\t");

  // Serial.print("before_ball ");
  // Serial.print(before_ball);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("gyro_deg ");
  // Serial.print(gyro_deg);
  // Serial.print("   ");
  // Serial.print("\t");

  // Serial.print("v_speed ");
  // Serial.print(v_speed);
  // Serial.print("   ");
  // Serial.print("\t");
  Serial.print("kick ");
  Serial.print(kick_time);
  Serial.print("   ");
  Serial.print("\t");

  Serial.println("\t");
}
