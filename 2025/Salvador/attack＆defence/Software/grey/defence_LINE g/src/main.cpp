#include <Arduino.h>
#include <timer.h>
#include <line.h>

#define PIN_DATA 0

#define PIN_S0 1
#define PIN_S1 2
#define PIN_S2 3
#define PIN_S3 4

#define PIN_E 5
////////////////////////ライン/////////////////////////////////////////////
int line_pins[] = {8, 7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9};
int line_pin_num;



int line_resarch[] = {122, 140, 137, 162, 222, 123, 125, 126, 152, 82, 82, 172, 112, 148, 162, 162};
int line_resarch_side[] = {115,90,15};
int line_vec_sheck;

float line_dir_sin;
float line_dir_cos;
int line_dir;

///////////////////////////////////////////////////////////////////16384

/////line////////////
// left 8 right 9 back10

#define left 8
#define right 9
#define back 10

float line_val_side = 0;
float No1_val_side = 0;

int line_right_val = 0;
int line_left_val = 0;
int line_back_val = 0;

int line_No1_side;

//////////////

float line_val = 0;
float No1_val = 0;
int line_no1 = 0;
int line_no = 0;

int line_No1;

int line_bit;

int line_bit_pre;
int line_dec = 0;


////閾値調査///

int line_re = 0;

int line_resarch_time;

bool line_resa = true;






void setup()
{
  Serial.begin(115200);

  pinMode(PIN_DATA, INPUT_PULLUP);
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);

  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);

  pinMode(PIN_E, OUTPUT);

  Serial1.begin(115200);
}


void loop()
{

  // 出力時は常にPIN_EはLOWにする必要がある
  digitalWrite(PIN_E, LOW);
  line_re = 0;
  if(line_resa){
    if (Serial1.available() > 0) // line
    {
      line_re = Serial1.readStringUntil('a').toInt();
    }

  }

  // Serial.print(line_re);
  // Serial.print(" ");



  if (line_re == 1)
  {
    if (line_resa)
    {

      LINE.start();
    }
  }else{
    LINE.tick();
    line_resarch_time = LINE.get_value();
    // Serial.print(line_resarch_time);
    // Serial.print(" ");
    if (line_resarch_time > 0 && line_resarch_time <= 2000)
    {

      for (int i = 0; i < 16; i++)
      {
        line_pin_num = line_pins[i];

        line_resarch[i] = readMux(line_pin_num) + 65; // Serialで映す用
      }

      for (int i = 8; i < 11; i++)
      {
        line_resarch_side[i - 8] = analogRead(i) + 15;//side
        
      }
      
      
      // Serial.print("aaaaaaaaaaaa");
    }
    else if(line_resarch_time > 2000)
    {
      line_resa = false;
      LINE.reset();
    }

  }

  line_dec = 0;
  line_bit = 0;
  line_bit_pre = 0;

  line_val = 0;

  line_No1 = 0;

  No1_val = 0;

  // 16ピン分のセンサーの値をマルチプレクサから取得する
  for (int i = 0; i < 16; i++)
  {
    line_pin_num = line_pins[i];

    line_read_analog[i] = readMux(line_pin_num); // Serialで映す用
    line_val = readMux(line_pin_num);

    line_no1 = line_pins[line_No1];

    No1_val = readMux(line_no1);

    if (line_val >= No1_val)
    {
      No1_val = line_val;
      line_No1 = i;
    }
    
    if (line_read_analog[i] > line_resarch[i])
    {
      line_read_digital[i] = 1;
      line_bit += pow(2, i);
      // if(i >= 0 && i <= 7){
      //   line_bit_pre += pow(2, i);

      // }else{
        
      //   line_bit += pow(2, i-8);
      // }
    }
    else
    {
      line_read_digital[i] = 0;
    }
  }


  /////////サイドライン/////
  line_left_val = analogRead(left);
  line_right_val = analogRead(right);
  line_back_val = analogRead(back);
  if(line_left_val > line_resarch_side[0]){
    line_read_digital_side[0] = 1;
  }else{
    line_read_digital_side[0] = 0;
  }

  
  if(line_right_val > line_resarch_side[1]){
    line_read_digital_side[1] = 1;
  }else{
    line_read_digital_side[1] = 0;
  }

  
  if(line_back_val > line_resarch_side[2]){
    line_read_digital_side[2] = 1;
  }else{
    line_read_digital_side[2] = 0;
  }

  if(line_read_digital_side[0] == 1){
    if(line_read_digital_side[1] == 1){
      if(line_read_digital_side[2] == 1){
        line_bit += 100000; //左　右　後

      }else{
        line_bit += 200000;///左　右

      }

    }else{
      if(line_read_digital_side[2] == 1){
        line_bit += 300000; //左　　後

      }else{
        line_bit += 400000;///左　

      }

    }
  }else{
    if(line_read_digital_side[1] == 1){
      if(line_read_digital_side[2] == 1){
        line_bit += 500000; //　右　後

      }else{
        line_bit += 600000;///　右

      }

    }else{
      if(line_read_digital_side[2] == 1){
        line_bit += 700000; //　　後

      }else{
        line_bit += 0;///　

      }

    }
  }


  Serial1.print(String(line_bit));
  Serial1.print(String('a'));
  Serial1.println(String("\t"));
  // Serial1.print(String(line_bit));
  // Serial1.print(String('b'));
  // Serial1.println(String("\t"));
  Serial1.flush();

  // Serial.print("ring");
  // for (int i = 0; i < 16; i++)
  // {
  //   Serial.print(line_resarch[i]);
  //   Serial.print(",");
  // }
  
  // Serial.print("side");
  // for (int i = 0; i < 3; i++)
  // {
  //   Serial.print(line_resarch_side[i]);
  // //   Serial.print(",");
  // // }
  

  
  Serial.print(analogRead(left));
  Serial.print(" ");
  Serial.print("\t");
  Serial.print(analogRead(right));
  // Serial.print(" ");
  Serial.print("\t");
  Serial.print(analogRead(back));
  Serial.print(" ");
  Serial.print("\t");
  // // // serial_print_side();
  serial_print_analog();
  serial_print_digital();
  // // // // // Serial.print(" ");
  // // // // // Serial.print(line_bit);
  // // // // // Serial.print(" ");
  // // // // // Serial.print(line_bit);
  Serial.println();
  
  

}
