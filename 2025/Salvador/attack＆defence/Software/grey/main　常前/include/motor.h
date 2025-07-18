#include <Arduino.h>
#include <acceleration.h>

int atack_goal_dir;
// int gyro_deg;
int gyro_data_pid;
float v_speed;

void breaking()
{
  Serial1.println("1R0002R0003R0004R000");
}

void move(int a, int b, int c, int d)
{
  int val = 0;
  int input[4] = {a, b, c, d};
  int output[4] = {0, 0, 0, 0};
  String StringA = "0";
  String StringB = "0";
  String StringC = "0";
  String StringD = "0";
  String StringX = "0";
  String StringY = "0";
  String StringZ = "0";
  String String0 = "0";

  StringX = String(1);
  val = input[0];
  if (val < 0 && val >= -100)
  {
    StringY = String('R');
    val = abs(val);
    StringZ = String(val);
  }
  else if (val >= 0 && val <= 100)
  {
    StringY = String('F');
    StringZ = String(val);
  }
  else
  {
    StringY = String('F');
    StringZ = String(0);
  }
  if (val < 10)
  {
    String0 = String("00");
    StringZ = String0 + StringZ;
  }
  else if (val < 100)
  {
    String0 = String('0');
    StringZ = String0 + StringZ;
  }
  else
  {
    String0 = String("");
  }
  StringA = StringX + StringY + StringZ;

  StringX = String(2);
  val = input[1];
  if (val < 0 && val >= -100)
  {
    StringY = String('R');
    val = abs(val);
    StringZ = String(val);
  }
  else if (val >= 0 && val <= 100)
  {
    StringY = String('F');
    StringZ = String(val);
  }
  else
  {
    StringY = String('F');
    StringZ = String(0);
  }
  if (val < 10)
  {
    String0 = String("00");
    StringZ = String0 + StringZ;
  }
  else if (val < 100)
  {
    String0 = String('0');
    StringZ = String0 + StringZ;
  }
  else
  {
  }
  StringB = StringX + StringY + StringZ;

  StringX = String(3);
  val = input[2];
  if (val < 0 && val >= -100)
  {
    StringY = String('R');
    val = abs(val);
    StringZ = String(val);
  }
  else if (val >= 0 && val <= 100)
  {
    StringY = String('F');
    StringZ = String(val);
  }
  else
  {
    StringY = String('F');
    StringZ = String(0);
  }
  if (val < 10)
  {
    String0 = String("00");
    StringZ = String0 + StringZ;
  }
  else if (val < 100)
  {
    String0 = String('0');
    StringZ = String0 + StringZ;
  }
  else
  {
  }
  StringC = StringX + StringY + StringZ;

  StringX = String(4);
  val = input[3];
  if (val < 0 && val >= -100)
  {
    StringY = String('R');
    val = abs(val);
    StringZ = String(val);
  }
  else if (val >= 0 && val <= 100)
  {
    StringY = String('F');
    StringZ = String(val);
  }
  else
  {
    StringY = String('F');
    StringZ = String(0);
  }
  if (val < 10)
  {
    String0 = String("00");
    StringZ = String0 + StringZ;
  }
  else if (val < 100)
  {
    String0 = String('0');
    StringZ = String0 + StringZ;
  }
  else
  {
  }
  StringD = StringX + StringY + StringZ;

  Serial1.println(StringA + StringB + StringC + StringD);
}

float get_max_value_in_array(float *ar) // 絶対値の個数
{
  float ret;

  for (int i = 0; i < 4; i++)
  {
    if (abs(ar[i] > ret))
    {
      ret = abs(ar[i]);
    }
  }

  return ret;
}

float val_P;
float val_D;
int PID_mode;

float pid_deviation; // 今回の実際角の偏差
float previous_pid_deviation;
int PID_gain(float P_gain, float D_gain, float P_gain_cam, int D_gain_cam, int outputdata, int PID_data, int limit)
// int PID_gain(float P_gain, float D_gain, float P_gain_cam, int D_gain_cam, int outputdata, int PID_data, int limit)
{
  int val_PID;
  int D_data;

  // 比例
  // val_P = (outputdata - 512.1) * P_gain;

  if (PID_mode == 1)
  {

    val_P = PID_data * P_gain_cam;
  }
  else
  {

    val_P = (outputdata)*P_gain; // マイナスけした
  }

  // 微分
  pid_deviation = val_P;
  val_D = pid_deviation - previous_pid_deviation;
  previous_pid_deviation = pid_deviation;

  if (PID_mode == 1)
  {

    D_data = D_gain_cam;
  }
  else
  {
    D_data = D_gain;
  }

  // 計算
  val_PID = (int)(max(min(val_P + val_D * D_gain, limit), -limit));

  return val_PID;
}

void Move_Deg(int deg, float power)
{
  float power_[4] = {0, 0, 0, 0};

  float multipli;
  float power_No1 = 0;
  float lost_power;

  int pid_val = PID_gain(1.5, 9, 0.2, 1.7, gyro_deg, atack_goal_dir, 40);
  // int pid_val = PID_gain(3.7, 8.5, 0.5, 9.5, gyro_deg, goal_dir, 40);
  // int pid_val = PID_gain(0.7,0.3, 0.5, 9.5, gyro_deg, goal_dir, 40);

  power_[0] = -sin((deg - 45) / 180.0 * 3.14) * power;  //+ pid_val
  power_[1] = sin((deg + 45) / 180.0 * 3.14) * power;   //- pid_val
  power_[2] = -sin((deg - 135) / 180.0 * 3.14) * power; //+ pid_val
  power_[3] = sin((deg + 135) / 180.0 * 3.14) * power;  //- pid_val

  for (int i = 0; i < 4; i++)
  {
    if (abs(power_[i]) >= power_No1)
    {
      power_No1 = abs(power_[i]);
    }
  }

  if (power_No1 != 0)
  {
    multipli = power / power_No1;

    for (int i = 0; i < 4; i++)
    {
      power_[i] = power_[i] * multipli;
    }
  }

  power_[0] += pid_val;
  power_[1] -= pid_val;
  power_[2] += pid_val;
  power_[3] -= pid_val;

  power_No1 = get_max_value_in_array(power_);

  if (power_No1 > 100)
  {
    lost_power = power_No1 - 100;

    for (int i = 0; i < 4; i++)
    {
      if (power_[i] > 0)
      {
        power_[i] = power_[i] - lost_power;
      }
      else
      {
        power_[i] = power_[i] + lost_power;
      }
    }
  }
  else
  {
    for (int i = 0; i < 4; i++)
    {
      power_[i] = max(min(power_[i], 100), -100);
    }
  }

  // Serial.print(power_[0]);
  // Serial.print("\t");
  // Serial.print(power_[1]);
  // Serial.print("\t");
  // Serial.print(power_[2]);
  // Serial.print("\t");
  // Serial.print(power_[3]);
  // Serial.print("\t");

  move((int)power_[0], (int)power_[1], (int)power_[2], (int)power_[3]);
}
