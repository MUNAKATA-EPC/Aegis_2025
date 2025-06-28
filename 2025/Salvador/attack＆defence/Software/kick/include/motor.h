

int goal_dir;
int gyro_data;
int speed_now;

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

float val_P;
float val_D;
int PID_mode;

float pid_deviation; // 今回の実際角の偏差
float previous_pid_deviation;
int PID_gain(float P_gain, float D_gain, float P_gain_cam, int D_gain_cam, int outputdata, int PID_data, int limit)
{
  int val_PID;
  int D_data;

  // 比例
  // val_P = (outputdata - 539) * P_gain;

  if (PID_mode == 1)
  {

    val_P = PID_data * P_gain_cam;
  }
  else
  {
    val_P = (outputdata)*P_gain; // マイナスけしたＸＤふぁe
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

void Move_Deg(int deg, int power)
{
  int pid_val = PID_gain(1.5, 9.5, 0.5, 9.5, gyro_data, goal_dir, 40);
  int power_A;
  int power_B;
  int power_C;
  int power_D;

  power_A = -sin((deg - 45) / 180.0 * 3.14) * power + pid_val;
  power_B = sin((deg + 45) / 180.0 * 3.14) * power - pid_val;
  power_C = -sin((deg - 135) / 180.0 * 3.14) * power + pid_val;
  power_D = sin((deg + 135) / 180.0 * 3.14) * power - pid_val;

  power_A = min(99, max(power_A, -99));
  power_B = min(99, max(power_B, -99));
  power_C = min(99, max(power_C, -99));
  power_D = min(99, max(power_D, -99));

  move((int)power_A, (int)power_B, (int)power_C, (int)power_D);

}