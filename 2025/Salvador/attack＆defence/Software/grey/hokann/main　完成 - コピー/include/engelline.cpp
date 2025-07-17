#include <Arduino.h>
#include <linecul.h>
#include "engelline.hpp"

double line_sensors_deg[16] = {
    22.5 * 9,
    22.5 * 10,
    22.5 * 11,
    22.5 * 12,
    22.5 * 13,
    22.5 * 14,
    22.5 * 15,
    22.5 * 0,
    22.5 * 1,
    22.5 * 2,
    22.5 * 3,
    22.5 * 4,
    22.5 * 5,
    22.5 * 6,
    22.5 * 7,
    22.5 * 8,
};

int line_deg;
int line_first_deg;
int previous_line_deg;

int line_evacuation_deg;

bool is_halfout;

bool is_exist_deg_value_in_range(int check_value, int base_value, int check_range)
{
    if (base_value - check_range < 0)
    {
        if (check_value <= base_value + check_range ||
            check_value >= base_value - check_range + 360)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (base_value + check_range > 360)
    {
        if (check_value <= base_value + check_range - 360 ||
            check_value >= base_value - check_range)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (check_value >= base_value - check_range &&
            check_value <= base_value + check_range)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// int get_count_of_detected_sensor()
// {
//     int ret = 0;

//     for(int i = 0; i < 16; i++)
//     {
//         if(line_data[i] == 1)
//         {
//             ret++;
//         }
//     }

//     return ret;
// }

bool is_line_detected() // trueならラインが検出されている、falseならラインが検出されていない
{
    bool ret = false;

    for (int i = 0; i < 16; i++)
    {
        if (line_data[i] == 1)
        {
            ret = true;
        }
    }

    return ret;
}

bool is_line_evacuation() // 白線に触れているか、半分外に出ているか？」を調べる関数です。
{
    return is_line_detected() || is_halfout;
}

bool is_previous_line_detected() // trueなら前回ラインが検出されていた、falseなら前回ラインが検出されていない
{
    return previous_line_deg != -1;
}

void process_engelline()
{

    if (is_line_detected())
    {
        // tone(3, 4000, 100);

        double line_vec_x = 0, line_vec_y = 0;

        for(int i = 0; i < 16; i++)
        {
            if (line_data[i] == 1)
            {
                line_vec_x += cos(radians(line_sensors_deg[i]));
                line_vec_y += sin(radians(line_sensors_deg[i]));
            }
        }

        //********************************************************ここ処理怪しいかもだから作り直した方がいい
        // line_deg = 360 - degrees(atan2(line_vec_y, line_vec_x));
        line_deg = degrees(atan2(line_vec_y, line_vec_x));

        line_deg += 180;

        // if(line_deg <= 360)
        // {
        //     line_deg = 360 - line_deg;
        // }
        // else
        // {
        //     line_deg = 360 - (line_deg - 180) + 180;
        // }

        if(abs(line_vec_x) < 0.01 && abs(line_vec_y) < 0.01)
        {
            line_deg = previous_line_deg;
        }
    }
    else
    {
        line_deg = -1;
    }

    // 今回初めてラインが反応したら
    if (is_line_detected() && !is_previous_line_detected())
    {
        if (!is_halfout)
        {
            line_first_deg = line_deg;
            previous_line_deg = line_deg;
            line_evacuation_deg = line_first_deg;

            return;
        }
    }

    // さっきまでラインが反応していたら
    if (!is_line_detected() && is_previous_line_detected())
    {
        if (!is_halfout)
        {
            line_first_deg = -1;
        }
    }

    // 継続してラインが反応していたら
    if (is_line_detected() && is_previous_line_detected())
    {
        // 前回とのラインの角度を比較して、もし大きく値がずれていたら「ハーフアウト」判定にする
        if (!is_exist_deg_value_in_range(line_deg, line_first_deg, 135))
        {
            is_halfout = true;
            
            // 基準角度と大きくずれていなかったら、基準角度を現在の角度に置き換え
            if (is_exist_deg_value_in_range((line_deg + 180) % 360, line_first_deg, 45))
            {
                line_first_deg = (line_deg + 180) % 360;
            }
        }
        else
        {
            is_halfout = false;

            // 基準角度と大きくずれていなかったら、基準角度を現在の角度に置き換え
            if (is_exist_deg_value_in_range(line_deg, line_first_deg, 45))
            {
                line_first_deg = line_deg;
            }
        }
    }

    previous_line_deg = line_deg;

    line_evacuation_deg = line_first_deg;
}
