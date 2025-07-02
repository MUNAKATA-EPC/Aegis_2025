#include <Arduino.h>


/// @brief デバッグ出力をする関数です。
/// @brief マルチプレクサーからの値を取得する関数です。
/// @param idx マルチプレクサから取得する値のインデックス。
/// @return 指定したインデックスのアナログ値。

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

  return analogRead(0);
}


int line_read_analog[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int line_read_digital[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int line_read_digital_side[] = {0,0,0};

void serial_print_analog()
{
  for (int i = 0; i < 16; i++)
  {
    Serial.print(line_read_analog[i]);
    Serial.print(", ");
  }
  Serial.print("\t");
}


void serial_print_digital()
{

  for (int i = 0; i < 16; i++)
  {
    Serial.print(line_read_digital[i]);
    Serial.print(", ");
  }
  Serial.print("\t");
}

void serial_print_side(){

  for (int i = 0; i < 3; i++)
  {
    Serial.print(line_read_digital_side[i]);
    Serial.print(" ");
  }
  



}