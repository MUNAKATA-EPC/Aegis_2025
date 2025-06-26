#include <Arduino.h>

#include <motor.h>
#include <line_.h>
// #include <timer.h>
#include <oled_main.h>

#define line_start_pin 2

#define STARTPIN 23
#define TOUCH 13
#define TEST 5
#define kicker 6
// #define mode_kick_test 
#define mode_atack_test 3//4

int touch = 0;
int a;
int b;
int Start = 0;
int serial_count = 0;
int move_speed = 0;
int start = 0;
int yellow_dis;
int blue_dis;

int atack_goal_dis;
int defence_goal_dis;
//////IR関連////////
int IR_dir;
int IR_dis;

//////oled関連/////
int oled_start_;
bool start_sceen = true;
int oled_start_time;

/////LINE閾値///////
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
int kick_time;

int com_mode = 0;

int rulu_time = 0;
int rulu_pre_time = 0;
bool KICK_fin = true;
int mode_atack_pin;
bool atack_test = false;
bool oled_reset = false;
int kick_test_time;

// 0,復帰状態　1,ゴールキーパー　2,アタッカー


void setup()
{
  pinMode(line_start_pin, INPUT_PULLDOWN);
  pinMode(STARTPIN, INPUT_PULLDOWN);
  pinMode(TOUCH, INPUT_PULLDOWN);
  pinMode(TEST, OUTPUT);
  pinMode(kicker, OUTPUT);
  // pinMode(mode_kick_test, INPUT_PULLDOWN);
  pinMode(mode_atack_test, INPUT_PULLDOWN);


  Serial.begin(9600);

  Serial1.begin(115200); // motor
  // Serial1.setTimeout(10);

  Serial2.begin(9600); // IR
  // Serial2.setTimeout(20);

  Serial4.begin(115200); //gyro

  Serial5.begin(115200); // line
  // Serial5.setTimeout(20);

  Serial3.begin(115200); // camera
  // Serial3.setTimeout(10);

  pinMode(resetpin, INPUT_PULLDOWN);

  oled_setup();

  // if (!bno.begin(OPERATION_MODE_IMUPLUS))
  // {
  //   while (1)

  //     // Serial.print("Ooops, no BNO055 detected ...")
  //     ;
  // }

  // bno.setExtCrystalUse(true);
}

void loop()
{

  digitalWrite(TEST, HIGH);
  // mode_change = digitalRead(mode_kick_test);
  mode_atack_pin = digitalRead(mode_atack_test);
  Serial.print("mode");
  Serial.print(mode_change);

  if(mode_atack_pin == 1){
    atack_test = true;
    com_mode = 2;
  }else{
    atack_test = false;
  }

  // line_bit = 0;
  oled_start_ = 0;
  mass_cou = 0;
  mass1 = 500;
  mass2 = 500;
  mass3 = 500;
  mode_line = 0;
  // goal_dir = 500;
  // court_dir = 500;

  serial_count += 1;

  ////9.1419
  if (serial_count < 3)
  {
    if (line_research == false)
    {
      while (Serial5.available() > 0) // line
      {
        line_bit = Serial5.readStringUntil('a').toInt();
      }
    }
    else
    {
      line_bit = 0;
    }
  }
  else if (serial_count >= 3 && serial_count < 5)
  {
    while (Serial2.available() > 0) // IR
    {
      IR_dir = Serial2.readStringUntil('a').toInt();
      IR_dis = Serial2.readStringUntil('b').toInt();
    }
  }
  else if (serial_count >= 5 && serial_count < 7)
  {
    while (Serial3.available() > 0) // camera
    {
      court_dir = Serial3.readStringUntil('a').toInt();
      yellow_dir = Serial3.readStringUntil('b').toInt();
      yellow_dis = Serial3.readStringUntil('c').toInt();
      blue_dir = Serial3.readStringUntil('d').toInt();
      blue_dis= Serial3.readStringUntil('e').toInt();
    }
  }
  else if (serial_count >= 7 && serial_count < 9)
  {
    while (Serial4.available() > 0) // gyro
    {
      gyro_data = Serial4.readStringUntil('a').toInt();
    }
  }
  else if (serial_count >= 9)
  {
    serial_count = 0;
  }

  defence_goal_dir = yellow_dir;
  defence_goal_dis = yellow_dis;
  atack_goal_dir = blue_dir;
  atack_goal_dis = blue_dis;


  // defence_goal_dir = blue_dir;
  // defence_goal_dis = blue_dis;
  // atack_goal_dir = yellow_dir;
  // atack_goal_dis = yellow_dis;

  // IR_rx 7line_rx21
  oled_button();
  Start = digitalRead(STARTPIN);
  Serial.print(Start);
  Serial.print(" ");
  line_start_ = digitalRead(line_start_pin);
  oled_start_ = digitalRead(oled_start_pin);
  touch = digitalRead(TOUCH);
  if (oled_start_ == 1)
  {
    oled_reset = false;
    if (start_sceen == true)
    {

      oled_aegis();
      start_sceen = false;
    }else if(start_sceen == false){
      OLED.start();
      OLED.tick();
      oled_start_time = OLED.get_value();
      if (oled_start_time > 50)
      {
        IR_dir_cal(IR_dir);
        Gyro_cal(gyro_data);
        Line_cal(line, line_bit);
        Camera_dir_cal(yellow_dir, blue_dir, court_dir);
        oled_main();
        oled_start_time = 0;
        OLED.reset();
      }

    }
  }
  else
  {
    if(oled_reset == false){
      reset_oled();
      oled_reset = true;
      screen_num = 0;
    }
  }
  // Serial.print("oled");
  // Serial.print(oled_start_);

  if (line_start_ == 1 || Start == 1)
  {
    if (line_research)
    {
      line_re = 1;
      Serial5.print(String(line_re));
      Serial5.println(String('a'));
      delay(1000);
      line_research = false;
    }
  }


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
  // IR_process();
  line_main();
  if(mass1 == mass2){
    mass2 = 500;
  }

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
    PID_mode = 0;
    move_speed = 45;
    if (com_mode == 0)
    {
      PID_mode = 0;
      if (line_start_ == 0)
      {

        if (defence_goal_dir == 500)
        {
          if (line_bit > 0)
          {
            Move_Deg(court_dir, 70);
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
              Move_Deg(court_dir, 70);
            }
            else
            {
              com_mode = 1;
            }
          }
          else
          {

            Move_Deg(defence_goal_dir, 40);
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
        if(defence_goal_dir > 90 && defence_goal_dis > 112){
          com_mode = 0;
        }else if(defence_goal_dir < -90 && defence_goal_dis > 112){
          com_mode = 0;
        }
        if (line_val == true)
        {
          
          
          if (mass1 < 500 && mass2 == 500 && mass3 == 500)
          {
            go_line = mass1;
            if(abs(defence_goal_dir - IR_dir) >= 45){
              if(IR_dir > 0){
                Move_Deg(mass1, 50);
              }else{
                Move_Deg(mass1, 20);
              }
            }else{
              Move_Deg(mass1, 90);
            }
          }
          else if (mass1 < 500 && mass2 < 500 && mass3 == 500)
          {
            rulu_time = RULU.get_value();
            
            if (abs(defence_goal_dir) >= 135)
            {
              if(IR_dis <= 8 ){
                if(goal_line_change - 45 < IR_dir && goal_line_change + 45 > IR_dir){//
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

            }
            else
            {
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
                if (goal_line_change > 5)
                {
                  Move_Deg(mass2, move_speed + 10);
                }
                else if (goal_line_change > -5)
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
                  Move_Deg(0, 0);
                }
                else
                {
                  if (goal_line_change - 10 > IR_dir)
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
            else if (mode_line == 4) //////////////////////////////////////////////////左右////////////mode4
            {
              if (IR_dir == 500)
              {
                if (goal_line_change > 5)
                {
                  Move_Deg(mass2, move_speed);
                }
                else if (goal_line_change > -5)
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
                if(IR_dis <= 8){
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

                }else{
                  if (goal_line_change - 170 > IR_dir)
                  {
                    Move_Deg(270, 20);
                  }
                  else if (goal_line_change -30  > IR_dir)
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
              if (abs(IR_dir) > 120)
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
          // if(abs(court_dir - goal_line_change) < 25){

          //   Move_Deg(goal_line_change, 40);

          // }else{
          //   Move_Deg(goal_dir, 40);

          // }
          Move_Deg(go_line, 40);
        }
      }
    }
    else if (com_mode == 2)
    {
      move_speed = 80;
      if(atack_goal_dir == 500){
        PID_mode = 0;

      }else{
        PID_mode = 1;
      }
      if(atack_test == false){
        RULU.start();
        RULU.tick();
      }
      rulu_time = RULU.get_value();
      if (rulu_time >= 1500)
      {
        com_mode = 0;
        RULU.reset();
      }
      else if(rulu_time >= 0 && rulu_time <= 300)
      {
        
        Move_Deg(IR_dir * 1.4, 70);
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
            if (IR_dir == 500)
            {
              Move_Deg(0, 0);
            }
            else
            {
              if (touch == 1)
              {
                KICK.start();
                KICK.tick();
                kick_time = KICK.get_value();
                if (kick_time >= 100)
                {
                  digitalWrite(TEST, LOW);
                  digitalWrite(kicker, HIGH);
                  delay(30);
                  digitalWrite(kicker, LOW);
                  KICK.reset();
                  kick_time = 0;
                  Move_Deg(0, move_speed);
                }else{
                  Move_Deg(0, move_speed);

                }
              }
              else
              {
                // KICK.reset();
                if (IR_dis < 500)
                {
                  if (IR_dir < 10)
                  {

                    Move_Deg(IR_dir * 1.4, move_speed);
                  }
                  else if (IR_dir < 30)
                  {

                    Move_Deg(IR_dir + 32, move_speed);
                  }
                  else if (IR_dir < 60)
                  {

                    Move_Deg(IR_dir + 70, move_speed);
                  }
                  else if (IR_dir < 160)
                  {

                    Move_Deg(IR_dir + 60, move_speed);
                  }
                  else if (IR_dir < 180)
                  {

                    Move_Deg(IR_dir + 60, move_speed);
                  }
                  else if (IR_dir < 210)
                  {

                    Move_Deg(IR_dir - 60, move_speed);
                  }
                  else if (IR_dir < 300)
                  {

                    Move_Deg(IR_dir - 60, move_speed);
                  }
                  else if (IR_dir < 330)
                  {

                    Move_Deg(IR_dir - 70, move_speed);
                  }
                  else if (IR_dir < 350)
                  {

                    Move_Deg(IR_dir - 40, move_speed);
                  }
                  else
                  {
                    IR_dir = IR_dir - 360;

                    Move_Deg(IR_dir * 1.4, move_speed);
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
  else
  {
    // if (mode_change == 1 && KICK_fin == true)
    // {
    //   KICK_TEST.start();
    //   KICK_TEST.tick();
    //   kick_test_time = KICK_TEST.get_value();
    //   if (kick_test_time >= 100 && kick_test_time <= 300)
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
    if (mode_change == 1)
    {
      KICK_TEST.start();
      KICK_TEST.tick();
      kick_test_time = KICK_TEST.get_value();
      if (kick_test_time >= 100 && kick_test_time <= 300)
      {
        digitalWrite(TEST, LOW);
        digitalWrite(kicker, HIGH);
        delay(30);
        digitalWrite(kicker, LOW);
        KICK_TEST.reset();
        kick_test_time = KICK_TEST.get_value();
        KICK_fin = false;
        mode_change = 0;
      }
    }
    move(0,0,0,0);

  }
  Serial.print("gyro,");
  Serial.print(gyro_data);
  Serial.print(" ");
  Serial.print("IR_dis,");
  Serial.print(IR_dis);
  Serial.print(" ");
  Serial.print("combat,");
  Serial.print(com_mode);
  Serial.print(" ");

  Serial.print("IR_dir,");
  Serial.print(IR_dir);
  Serial.print(" ");
  // Serial.print("yellow_dis,");
  // Serial.print(yellow_dis);
  // Serial.print(" ");
  Serial.print("yellow_dir,");
  Serial.print(yellow_dir);
  Serial.print(" ");
  // Serial.print("blue_dis,");
  // Serial.print(blue_dis);
  // Serial.print(" ");
  // Serial.print("blue_dir,");
  // Serial.print(blue_dir);
  // Serial.print(" ");
  // // Serial.print("ball_time,");
  // // Serial.print(ball_time);
  // // Serial.print(" ");
  // Serial.print("line_time,");
  // Serial.print(line_time);
  // Serial.print(" ");

  // Serial.print("goline,");
  // Serial.print(go_line);
  Serial.print("rulu_time,");
  Serial.print(rulu_time);
  Serial.print(" ");
  // Serial.print("kick_time,");
  // Serial.print(kick_test_time);
  // Serial.print(" ");
  // Serial.print("com,");
  // Serial.print(com_mode);
  // Serial.print(" ");
  // Serial.print("PID,");
  // Serial.print(PID_mode);
  // Serial.print(" ");
  // Serial.print("court,");
  // Serial.print(court_dir);
  // Serial.print(" ");
  // Serial.print("mode_chan,");
  // Serial.print(mode_change);
  // Serial.print(" ");

  Serial.println(" ");
}