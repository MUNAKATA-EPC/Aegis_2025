#include <Arduino.h>
#include <Wire.h>
//旧ジャイロ
//シリアルモニタを起動しなければプログラムが走らない！！！

// Connect L3GD20 with SDA (A4), SCL (A5)

const byte L3GD20_ADDR = B1101010;  // SA0 = GND
//const byte L3GD20_ADDR = B1101011;// SA0 = VDD_IO

const byte L3GD20_WHOAMI = 0x0F;
const byte L3GD20_CTRL1 = 0x20;
const byte L3GD20_CTRL2 = 0x21;
const byte L3GD20_CTRL3 = 0x22;
const byte L3GD20_CTRL4 = 0x23;
const byte L3GD20_CTRL5 = 0x24;
const byte L3GD20_X_L = 0x28;
const byte L3GD20_X_H = 0x29;
const byte L3GD20_Y_L = 0x2A;
const byte L3GD20_Y_H = 0x2B;
const byte L3GD20_Z_L = 0x2C;
const byte L3GD20_Z_H = 0x2D;

#define ledpin 13
#define ledpin2 12
#define analogpin 6
#define outputpin 0
#define resetpin 7
#define CAL_TIMEMS 1000
#define UPDATE_MS 10

float microcaldata;
float degdata = 0;
float caldata = 0;//←ここにキャリブレーション値のデフォルトを

float z_old;

int current_time;
int last_time;

bool bCalNow;

void Writegyro(byte reg, byte val)
{
  Wire.beginTransmission(L3GD20_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();  
}

byte Readgyro(byte reg)
{
  byte ret = 0;
  Wire.beginTransmission(L3GD20_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((unsigned int)L3GD20_ADDR, 1);
  
  while (Wire.available()) {
    ret = Wire.read();
  }
  
  return ret;
}

void GetDegdata(){
  short Z = Readgyro(L3GD20_Z_H);
  float z = Z = (Z << 8) | Readgyro(L3GD20_Z_L);

  microcaldata = analogRead(analogpin);
  microcaldata = (microcaldata - 512) /  50 + 20;
  
  float z_real = (z + z_old) / 2;
  z_old = z;

  degdata += z_real + (bCalNow ? 0 : caldata) ;//degdata = degdata + z + ();
  degdata += microcaldata;
}

void Calibration(){
  bCalNow = true;

  float before_deg;
  float now_deg;

  Serial.println("####[Calibration Started]####");

  before_deg = degdata;

  for(int i = 0; i < (CAL_TIMEMS / UPDATE_MS); i++){
    GetDegdata();
    delay(UPDATE_MS);
  }
  
  now_deg = degdata;

  caldata = (before_deg - now_deg) / (CAL_TIMEMS / UPDATE_MS);

  degdata = 0;
  bCalNow = false;
  
  Serial.print("####[Calibration Ended value = ");
  Serial.print(caldata);
  Serial.print("]####");
}

void setup() {
  analogWriteResolution(10);
  
  pinMode(resetpin, INPUT_PULLDOWN);
  pinMode(ledpin, OUTPUT);
  pinMode(ledpin2, OUTPUT);

  Serial.begin(9600);
  Wire.begin();
  
  Writegyro(L3GD20_CTRL1, B00001111);
                      //   |||||||+ X axis enable
                      //   ||||||+- Y axis enable
                      //   |||||+-- Z axis enable
                      //   ||||+--- PD: 0: power down, 1: active:]
                      //   ||++---- BW1-BW0: cut off 12.5[Hz]
                      //   ++------ DR1-DR0: ODR 95[HZ]

  Writegyro(L3GD20_CTRL4, B00110000);
}


void loop() {
  if(digitalRead(resetpin) == HIGH){        //リセットボタンの処理
    degdata = 0;
  }

  GetDegdata();

  float outputdata = (degdata / 480) + 512;
  if(outputdata > 1023 ){
    outputdata = 1023;
  } else if(outputdata < 0){
    outputdata = 0;     
  }

  Serial.print(microcaldata);
  Serial.print(" ");
  Serial.println(outputdata);

  analogWrite(outputpin, outputdata);

  delay(UPDATE_MS);
}/////kyuua