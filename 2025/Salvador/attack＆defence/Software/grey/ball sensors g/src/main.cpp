#include <Arduino.h>
#include <average.h>

// Mux 制御ピン
#define s0 1
#define s1 2
#define s2 3
#define s3 4
#define PIN_E 5

#define DETECTED_CHECK_VALUE 960

// 「SIG」ピンをMuxする
#define SIG_pin 0

float IR_val;

int IR_No1;
int IR_pra1;
int IR_pra2;
int IR_pra3;
int IR_minu1;
int IR_minu2;
int IR_minu3;

float No1_val;
float No1_val_cos;
float IR_pra1_cos;
float IR_pra2_cos;
float IR_pra3_cos;
float IR_minu1_cos;
float IR_minu2_cos;
float IR_minu3_cos;

float No1_val_sin;
float IR_pra1_sin;
float IR_pra2_sin;
float IR_pra3_sin;
float IR_minu1_sin;
float IR_minu2_sin;
float IR_minu3_sin;

float sin_ful_dir;
float cos_ful_dir;

float sin_ful_dis;
float cos_ful_dis;

float IR_dir = 500;
float IR_dis = 500;

float readMux(int channel)
{
  uint8_t mul_address = channel;
  uint8_t bit;

  for (uint8_t j = 0; j < 4; j++)
  {
    bit = 1 << j;

    if (bit & mul_address)
    {
      digitalWrite(j + 1, HIGH);
    }
    else
    {
      digitalWrite(j + 1, LOW);
    }
  }

  delayMicroseconds(10);

  return analogRead(SIG_pin);
}

void setup()
{
  analogReadResolution(10);
  analogWriteResolution(10);

  pinMode(SIG_pin, INPUT_PULLUP);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  pinMode(PIN_E, OUTPUT);

  Serial.begin(115200);

  Serial1.begin(115200);
}

void loop()
{
  digitalWrite(PIN_E, LOW);

  int IR_No[] = {
      13,
      14,
      15,
      0,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      9,
      10,
      11,
      12,
      13,
      14,
      15,
      0,
      1,
      2,
  };
  int IR_pi[] = {6, 7, 8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -7, -6, -5};

  int ir_data[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  IR_val = 1023;
  // IR_No1 = 15;

  No1_val = 1023;

  // // ループして 16 個の値をすべて読み取ります//報告します チャネル 6 の値は 346 です

  for (int i = 0; i <= 15; i++)
  {
    IR_val = readMux(i);
    No1_val = readMux(IR_No1);

    ir_data[i] = IR_val;

    if (IR_val <= No1_val)
    {
      No1_val = IR_val;
      if (No1_val > 950)
      {
        IR_No1 = 100;
      }
      else
      {
        IR_No1 = i;
      }
    }
  }

  if (No1_val > 950)
  {
    IR_dir = 500;
    IR_dis = 500;
  }
  else
  {

    IR_No1 += 3;
    IR_pra1 = IR_No1 + 1;
    IR_pra2 = IR_No1 + 2;
    IR_pra3 = IR_No1 + 3;
    IR_minu1 = IR_No1 - 1;
    IR_minu2 = IR_No1 - 2;
    IR_minu3 = IR_No1 - 3;

    No1_val_cos = No1.moving_average(readMux(IR_No[IR_No1]));

    IR_pra1_cos = Nopra1.moving_average(readMux(IR_No[IR_pra1]));
    IR_pra2_cos = Nopra2.moving_average(readMux(IR_No[IR_pra2]));
    IR_pra3_cos = Nopra3.moving_average(readMux(IR_No[IR_pra3]));

    IR_minu1_cos = Nominu1.moving_average(readMux(IR_No[IR_minu1]));
    IR_minu2_cos = Nominu2.moving_average(readMux(IR_No[IR_minu2]));
    IR_minu3_cos = Nominu3.moving_average(readMux(IR_No[IR_minu3]));

    No1_val_sin = No1_val_cos;

    IR_pra1_sin = IR_pra1_cos;
    IR_pra2_sin = IR_pra2_cos;
    IR_pra3_sin = IR_pra3_cos;

    IR_minu1_sin = IR_minu1_cos;
    IR_minu2_sin = IR_minu2_cos;
    IR_minu3_sin = IR_minu3_cos;

    /*No1_val_sin = moving_average(readMux(IR_No[IR_No1]));

    IR_pra1_sin = moving_average(readMux(IR_No[IR_pra1]));
    IR_pra2_sin = moving_average(readMux(IR_No[IR_pra2]));
    IR_pra3_sin = moving_average(readMux(IR_No[IR_pra3]));

    IR_minu1_sin = moving_average(readMux(IR_No[IR_minu1]));
    IR_minu2_sin = moving_average(readMux(IR_No[IR_minu2]));
    IR_minu3_sin = moving_average(readMux(IR_No[IR_minu3]));*/

    No1_val_cos = No1_val_cos * cos(radians(IR_pi[IR_No1] * 22.5));

    IR_pra1_cos = IR_pra1_cos * cos(radians(IR_pi[IR_pra1] * 22.5));
    IR_pra2_cos = IR_pra2_cos * cos(radians(IR_pi[IR_pra2] * 22.5));
    IR_pra3_cos = IR_pra3_cos * cos(radians(IR_pi[IR_pra3] * 22.5));

    IR_minu1_cos = IR_minu1_cos * cos(radians(IR_pi[IR_minu1] * 22.5));
    IR_minu2_cos = IR_minu2_cos * cos(radians(IR_pi[IR_minu2] * 22.5));
    IR_minu3_cos = IR_minu3_cos * cos(radians(IR_pi[IR_minu3] * 22.5));

    No1_val_sin = No1_val_sin * sin(radians(IR_pi[IR_No1] * 22.5));

    IR_pra1_sin = IR_pra1_sin * sin(radians(IR_pi[IR_pra1] * 22.5));
    IR_pra2_sin = IR_pra2_sin * sin(radians(IR_pi[IR_pra2] * 22.5));
    IR_pra3_sin = IR_pra3_sin * sin(radians(IR_pi[IR_pra3] * 22.5));

    IR_minu1_sin = IR_minu1_sin * sin(radians(IR_pi[IR_minu1] * 22.5));
    IR_minu2_sin = IR_minu2_sin * sin(radians(IR_pi[IR_minu2] * 22.5));
    IR_minu3_sin = IR_minu3_sin * sin(radians(IR_pi[IR_minu3] * 22.5));

    sin_ful_dir = No1_val_sin + IR_pra1_sin + IR_minu1_sin;
    cos_ful_dir = No1_val_cos + IR_pra1_cos + IR_minu1_cos;

    // sin_ful = No1_val_sin + IR_pra1_sin + IR_pra2_sin  + IR_minu1_sin + IR_minu2_sin ;
    // cos_ful = No1_val_cos + IR_pra1_cos + IR_pra2_cos  + IR_minu1_cos + IR_minu2_cos ;

    sin_ful_dis = No1_val_sin + IR_pra1_sin + IR_pra2_sin + IR_pra3_sin + IR_minu1_sin + IR_minu2_sin + IR_minu3_sin;
    cos_ful_dis = No1_val_cos + IR_pra1_cos + IR_pra2_cos + IR_pra3_cos + IR_minu1_cos + IR_minu2_cos + IR_minu3_cos;

    IR_dir = atan2(sin_ful_dir, cos_ful_dir) * 180 / PI;

    IR_dis = sqrt(pow(sin_ful_dis, 2) + pow(cos_ful_dis, 2)) / 100;

    IR_dis = IR_dis_ave.moving_average(IR_dis);
  }
  Serial1.print(String(IR_dir));
  Serial1.print(String("a"));
  Serial1.println(String("\t"));
  Serial1.print(String(IR_dis));
  Serial1.print(String("b"));
  Serial1.println(String("\t"));
  Serial1.flush();

  Serial.print(IR_No1);
  Serial.print("\t");
  Serial.print(IR_dis);
  Serial.print("\t");
  Serial.print(IR_dir);
  Serial.print("\t");

  for (int i = 0; i < 16; i++)
  {
    Serial.print(ir_data[i]);
    Serial.print("\t");
  }
  Serial.println();
}
