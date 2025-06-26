#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <timer.h>

//////////////画像/////////////////
#include <oled_aegis.h>

// 左10　真ん中12　右11

#define left_button 10
#define right_button 11
#define center_button 12

// コンパイル時にヘッダーファイルが適切に編集されていない場合に
// "Height incorrect, please fix Adafruit_SSD1306.h!"
// というエラーを表示するための記述
#if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// RSTピンがない互換品を使用するので-1を指定
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define oled_start_pin 4
int IR_dir_x;
int IR_dir_y;
int IR_dir_screen;

int gyro_x;
int gyro_y;
int gyro_screen;

int line_x;
int line_y;
int line_screen;
int line_det;

int yellow_x;
int yellow_y;
int yellow_screen;

int blue_x;
int blue_y;
int blue_screen;

int green_x;
int green_y;
int green_screen;

int screen_num = 0;
int floar;

int left_total;
int right_total;
int center_total;


int mode_change = 0;

void oled_aegis()
{

  // 画面表示をクリア
  display.clearDisplay();

  display.drawBitmap(0, 0, bitmap_aegis, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

  // 描画バッファの内容を画面に表示
  display.display();
}
void IR_dir_cal(int IR_dir){
  IR_dir_x = sin(radians(IR_dir)) * 20;
  IR_dir_y = cos(radians(IR_dir)) * 20;
  if(IR_dir == 500){
    IR_dir_x = 0;
    IR_dir_y = 0;
  }
  IR_dir_screen = IR_dir;
}

void Gyro_cal(int gyro_dir){
  gyro_x = sin(radians(gyro_dir)) * 20;
  gyro_y = cos(radians(gyro_dir)) * 20;

  gyro_screen = gyro_dir;

}

void Line_cal(int line, int line_dir){
  line_x = sin(radians(line_dir)) * 20;
  line_y = cos(radians(line_dir)) * 20;

  line_screen = line_dir;
  line_det = line;
  if(line_dir == 500){
    line_x = 0;
    line_y = 0;
  }

}
void Camera_dir_cal(int goal_yellow,int goal_blue,int cout_green){
  yellow_x = sin(radians(goal_yellow)) * 20;
  yellow_y = cos(radians(goal_yellow)) * 20;

  yellow_screen = goal_yellow;

  blue_x = sin(radians(goal_blue)) * 20;
  blue_y = cos(radians(goal_blue)) * 20;

  blue_screen = goal_blue;

  green_x = sin(radians(cout_green)) * 20;
  green_y = cos(radians(cout_green)) * 20;

  green_screen = cout_green;

}

void oled_setup()
{
  // I2Cアドレスは使用するディスプレイに合わせて変更する
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(left_button, INPUT_PULLDOWN);
  pinMode(right_button, INPUT_PULLDOWN);
  pinMode(center_button, INPUT_PULLDOWN);
  pinMode(oled_start_pin, INPUT_PULLDOWN);
}

#define max_sceen_num_0 3
#define max_sceen_num_1 7
#define max_sceen_num_2 1
#define max_sceen_num_3 2
bool button_cool = true;
void oled_button()
{
  int button_time_left;
  int button_time_right;
  int button_time_center;
  bool button;

  left_total = digitalRead(left_button);
  right_total = digitalRead(right_button);
  center_total = digitalRead(center_button);
  
  button_time_left = BUTTON_left.get_value();
  BUTTON_left.tick();

  button_time_right = BUTTON_right.get_value();
  BUTTON_right.tick();

  button_time_center = BUTTON_center.get_value();
  BUTTON_center.tick();
  if(button_time_left == 0 && button_time_right == 0 && button_time_center == 0){
    button = true;
  }else{
    button = false;
  }
  if(screen_num == 0){
    if(button == true){
      if(button_time_left == 0){
        if(left_total == 1){
          BUTTON_left.start();
          if(button_cool){
            floar -= 1;
            button_cool = false;
          }
        }
      }
      if(button_time_right == 0){
        if(right_total == 1){
          BUTTON_right.start();
          if(button_cool){
            floar += 1;
            button_cool = false;
          }
        }
      }
      if(button_time_center == 0){
        if(center_total == 1){
          BUTTON_center.start();
          if(button_cool){
            if(floar == 0){
              screen_num = 1;
              floar = 0;
            }else if(floar == 1){
              screen_num = 2;
              floar = 0;
            }else if(floar == 2){
              screen_num = 3;
              floar = 0;
            }
            button_cool = false;
          }
        }
      }
  
    }else{
      if(button_time_left >= 500){
        BUTTON_left.reset();
        button_cool = true;
      }
      if(button_time_right >= 500){
        BUTTON_right.reset();
        button_cool = true;
      }
      if(button_time_center >= 500){
        BUTTON_center.reset();
        button_cool = true;
      }
    }
    if (floar <= -1)
    {
      floar = max_sceen_num_0 - 1;
    }
    else if (floar >= max_sceen_num_0)
    {
      floar = 0;
    }
  }else if(screen_num == 1){
    if(button == true){
      if(button_time_left == 0){
        if(left_total == 1){
          BUTTON_left.start();
          if(button_cool){
            floar -= 1;
            button_cool = false;
          }
        }
      }
      if(button_time_right == 0){
        if(right_total == 1){
          BUTTON_right.start();
          if(button_cool){
            floar += 1;
            button_cool = false;
          }
        }
      }
      if(button_time_center == 0){
        if(center_total == 1){
          BUTTON_center.start();
          if(button_cool){
            if(floar == 6){
              screen_num = 0;
            }
            button_cool = false;
          }
        }
      }
  
    }else{
      if(button_time_left >= 500){
        BUTTON_left.reset();
        button_cool = true;
      }
      if(button_time_right >= 500){
        BUTTON_right.reset();
        button_cool = true;
      }
      if(button_time_center >= 500){
        BUTTON_center.reset();
        button_cool = true;
      }
    }
    if (floar <= -1)
    {
      floar = max_sceen_num_1 - 1;
    }
    else if (floar >= max_sceen_num_1)
    {
      floar = 0;
    }

  }else if(screen_num == 2){
    if(button == true){
      if(button_time_left == 0){
        if(left_total == 1){
          BUTTON_left.start();
          if(button_cool){
            floar -= 1;
            button_cool = false;
          }
        }
      }
      if(button_time_right == 0){
        if(right_total == 1){
          BUTTON_right.start();
          if(button_cool){
            floar += 1;
            button_cool = false;
          }
        }
      }
      if(button_time_center == 0){
        if(center_total == 1){
          BUTTON_center.start();
          if(button_cool){
            if(floar == 0){
              screen_num = 0;
            }
            button_cool = false;
          }
        }
      }
  
    }else{
      if(button_time_left >= 500){
        BUTTON_left.reset();
        button_cool = true;
      }
      if(button_time_right >= 500){
        BUTTON_right.reset();
        button_cool = true;
      }
      if(button_time_center >= 500){
        BUTTON_center.reset();
        button_cool = true;
      }
    }
    if (floar <= -1)
    {
      floar = max_sceen_num_2 - 1;
    }
    else if (floar >= max_sceen_num_2)
    {
      floar = 0;
    }

  }else if(screen_num == 3){
    if(button == true){
      if(button_time_left == 0){
        if(left_total == 1){
          BUTTON_left.start();
          if(button_cool){
            floar -= 1;
            button_cool = false;
          }
        }
      }
      if(button_time_right == 0){
        if(right_total == 1){
          BUTTON_right.start();
          if(button_cool){
            floar += 1;
            button_cool = false;
          }
        }
      }
      if(button_time_center == 0){
        if(center_total == 1){
          BUTTON_center.start();
          if(button_cool){
            if(floar == 0){
              if(mode_change == 0){
                mode_change = 1;
              }
            }
            else if(floar == 1)
            {
              screen_num = 0;
            }
            button_cool = false;
          }
        }
      }
  
    }else{
      if(button_time_left >= 500){
        BUTTON_left.reset();
        button_cool = true;
      }
      if(button_time_right >= 500){
        BUTTON_right.reset();
        button_cool = true;
      }
      if(button_time_center >= 500){
        BUTTON_center.reset();
        button_cool = true;
      }
    }
    if (floar <= -1)
    {
      floar = max_sceen_num_3 - 1;
    }
    else if (floar >= max_sceen_num_3)
    {
      floar = 0;
    }

  }
  //   Serial.print(left_total);
  //   Serial.print("  ");
  // Serial.println(right_total);
  Serial.print(screen_num);
  Serial.print("  ");
  Serial.print(button_time_left);
  Serial.print("  ");
  Serial.print(left_total);
  Serial.print("  ");

  

  //   if(right_total > 300){
  //     floar += 1;
  //     right_total = 0;
  //   }
}

void oled_main()
{

  display.clearDisplay();
  if (screen_num == 0)
  {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 12);
    display.println(("Serial Monitor"));

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 24);
    display.println(("Program"));

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 36);
    display.println(("Check"));

    display.fillTriangle(15, 55, 10, 62, 20, 62, SSD1306_WHITE);
    display.fillTriangle(113, 62, 108, 55, 118, 55, SSD1306_WHITE);
    display.drawCircle(64,57,6,SSD1306_WHITE);
    display.drawLine(61,57, 64, 60, SSD1306_WHITE);
    display.drawLine(67,53, 64, 60, SSD1306_WHITE);
    Serial.print("floar");
    Serial.print(" ");
    Serial.print(floar);
    if (floar == 0)
    {
      display.fillTriangle(15, 8, 15, 18, 22, 13, SSD1306_WHITE);
    }
    else if (floar == 1)
    {
      display.fillTriangle(15, 20, 15, 30, 22, 25, SSD1306_WHITE);
    }
    else if (floar == 2)
    {
      display.fillTriangle(15, 32, 15, 42, 22, 37, SSD1306_WHITE);
    }
  }else if(screen_num == 1){
      if (floar == 0){
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(60, 48);
      display.println(("IR"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("HOME"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(95, 48);
      display.println(("gyro"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((IR_dir_screen));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + IR_dir_x, 25 - IR_dir_y, SSD1306_WHITE);
  
      }else if(floar == 1){
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(50, 48);
      display.println(("gyro"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("IR"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(95, 48);
      display.println(("line"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((gyro_screen));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + gyro_x, 25 - gyro_y, SSD1306_WHITE);
  
      }else if(floar == 2){
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(48, 48);
      display.println(("line"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("gyro"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(85, 48);
      display.println(("yellow"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((line_screen));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 30);
      display.println((line_det));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + line_x, 25 - line_y, SSD1306_WHITE);
  
      }else if(floar == 3){
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(39, 48);
      display.println(("yellow"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("line"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(95, 48);
      display.println(("blue"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((yellow_screen));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + yellow_x, 25 - yellow_y, SSD1306_WHITE);
  
      }else if(floar == 4){
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(60, 48);
      display.println(("blue"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("yellow"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(95, 48);
      display.println(("court"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((blue_screen));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + blue_x, 25 - blue_y, SSD1306_WHITE);
      }else if(floar == 5){
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(48, 48);
      display.println(("court"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 48);
      display.println(("blue"));
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(95, 48);
      display.println(("HOME"));
  
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 20);
      display.println((green_screen));
  
      display.drawCircle(64, 25, 20, SSD1306_WHITE);
  
      display.drawLine(64, 25, 64 + green_x, 25 - green_y, SSD1306_WHITE);
  
      }else if(floar == 6){
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(40, 25);
        display.println(("HOME"));
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 48);
        display.println(("court"));
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(95, 48);
        display.println(("IR"));
    

      }
  }else if(screen_num == 2){
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 25);
    display.println(("comeing soon"));
  }else if(screen_num == 3){
    if(floar == 0){
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(40, 25);
      display.println(("kick_test"));
      if(mode_change == 0){
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(48, 48);
        display.println(("false"));
      }else{
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(48, 48);
        display.println(("true"));
      }
    }else if(floar == 1){
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(40, 25);
      display.println(("HOME"));
    }
  }


  display.display();
}

void reset_oled()
{

  display.clearDisplay();
  display.display();
}
