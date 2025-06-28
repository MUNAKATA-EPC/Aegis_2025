#include <Arduino.h>
class average
{
private:
  bool now_detected;
  bool previous_detected;
  int stock_rotation_term = 30; // 移動平均の期間（標本数）
  int stock_rotation_number = 0; // 輪番用のカウント
  float stock_data[30];         // データをストックする配列
  float new_data;                // センサから取得した最新のデータ

  bool ave = false;
  float result;
public:
  float moving_average(float data)
  { // IMUデータの移動平均値の取得
  // 二次元配列の0番から輪番で最新のデータを入れていく。指定個数を上回ったら0番に戻す
    if (stock_rotation_number >= stock_rotation_term)
    {
      stock_rotation_number = 0;
      ave = true;

    }

    stock_data[stock_rotation_number] = data; // ストックの輪番箇所にデータ上書き

    result = 0; // 結果を初期化
    if(ave == true){
      for (int i = 0; i < stock_rotation_term-1; i++)
      { // ストックした値を合計する
        result += stock_data[i];
      }

      result = result / stock_rotation_term; // 合計値を標本数でを割る

    }else{
      for (int i = 0; i <= stock_rotation_number; i++)
      { // ストックした値を合計する
        result += stock_data[i];
      }

      result = result / (stock_rotation_number + 1); // 合計値を標本数でを割る

    }


    stock_rotation_number += 1;

    return result;
  }
};
average linecenterval;
average linerightval;
average lineleftval;
average linebackval;
average leftcenterval;
average rightcenterval;
average leftbackval;
average rightbackval;


