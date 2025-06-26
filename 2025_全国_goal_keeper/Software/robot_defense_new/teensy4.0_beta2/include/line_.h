#include <Arduino.h>
//////ライン関連//////////
int line;
int line_time;
int line_bit;
int line_bit_cul;
int line_data[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int line_dir_num[] = {0, 22, 45, 67, 90, 112, 135, 157, 180, -157, -135, -112, -90, -67, -45, -22};
////float line_dir_num[] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, -157.5, -135, -112.5, -90, -67.5, -45, -22.5};
int side_line;

float mass1;
float mass2;
float mass3;
bool mass_check = false;
int mass_pre;
int mass_lat;
int mass_cou = 0;
int mode_line;

void line_main()
{

    // Serial.print("line_pre,");
    // Serial.print(line_bit_pre);
    // Serial.print(" ");
    // Serial.print("line_bit,");
    // Serial.print(line_bit);
    // Serial.print(" ");

    line_bit_cul = line_bit;
    if (line_bit_cul >= 100000)
    { // 上から見たとき
        if (line_bit_cul >= 200000)
        {
            if (line_bit_cul >= 300000)
            {
                if (line_bit_cul >= 400000)
                {
                    if (line_bit_cul >= 500000)
                    {
                        if (line_bit_cul >= 600000)
                        {
                            if (line_bit_cul >= 700000)
                            { // 　　後　
                                side_line = 7;
                                line_bit_cul -= 700000;
                                    mode_line = 3;
                                
                            }
                            else
                            { // 　左
                                side_line = 6;
                                line_bit_cul -= 600000;
                                mode_line = 0;
                            }
                        }
                        else
                        { // 　左　後
                            side_line = 5;
                            line_bit_cul -= 500000;
                            // mode_line = 2;
                            mode_line = 0;
                        }
                    }
                    else
                    { // 右　
                        side_line = 4;
                        line_bit_cul -= 400000;
                        mode_line = 0;
                    }
                }
                else
                { // 右　　後
                    side_line = 3;
                    line_bit_cul -= 300000;
                    // mode_line = 1;
                    mode_line = 0;
                }
            }
            else
            { // 左　右
                side_line = 2;
                line_bit_cul -= 200000;
                mode_line = 4;
            }
        }
        else
        { // 右　左　後
            side_line = 1;
            line_bit_cul -= 100000;
            mode_line = 0;
        }
    }
    else
    {
        side_line = 0;
        line_bit_cul += 0;
        // if (line_bit_cul > 0)
        // {
        //     mode_line = 5;
        // }else{
            
            mode_line = 0;
        // }
    }
    // Serial.print("bit_cul,");
    // Serial.print(line_bit_cul);
    // Serial.print(" ");

    for (int i = 0; i <= 15; i++)
    {

        if (line_bit_cul >= pow(2, 15 - i))
        {
            line_data[15 - i] = 1;
            line_bit_cul -= pow(2, 15 - i);
        }
        else
        {
            line_data[15 - i] = 0;
        }

        // Serial.print(line_data[i]);
        // Serial.print(", ");
    }
    mass_pre = 0;
    mass_lat = 0;
    for (int i = 0; i < 16; i++)
    {
        if (line_data[i] == 1)
        {
            if (mass_check == false)
            {

                mass_pre = i;
                if (line_data[i + 1] == 0 || i + 1 == 16)
                {
                    mass_cou += 1;
                    if (mass_cou == 1)
                    {
                        mass1 = mass_pre * 22.5;
                    }
                    else if (mass_cou == 2)
                    {
                        mass2 = mass_pre * 22.5;
                    }
                    else if (mass_cou == 3)
                    {
                        mass3 = mass_pre * 22.5;
                    }
                }
                else
                {
                    mass_check = true;
                }
            }
            else
            {
                if (i == 15)
                {
                    mass_lat = 15;

                    mass_cou += 1;
                    if (mass_cou == 1)
                    {
                        mass1 = (mass_pre + mass_lat) / 2 * 22.5;
                    }
                    else if (mass_cou == 2)
                    {
                        mass2 = (mass_pre + mass_lat) / 2 * 22.5;
                    }
                    else if (mass_cou == 3)
                    {
                        mass3 = (mass_pre + mass_lat) / 2 * 22.5;
                    }

                    mass_check = false;
                }
            }
        }
        else
        {
            if (mass_check)
            {
                if (line_data[i] == 0)
                {

                    mass_lat = i - 1;

                    mass_cou += 1;
                    if (mass_cou == 1)
                    {
                        mass1 = (mass_pre + mass_lat) / 2 * 22.5;
                    }
                    else if (mass_cou == 2)
                    {
                        mass2 = (mass_pre + mass_lat) / 2 * 22.5;
                    }
                    else if (mass_cou == 3)
                    {
                        mass3 = (mass_pre + mass_lat) / 2 * 22.5;
                    }

                    mass_check = false;
                }
            }
        }
    }

    if (line_data[0] == 1 && line_data[15] == 1)
    {
        if (mass_cou == 2)
        {
            mass1 = ((mass1 + 360) + mass2) / 2;
            mass2 = mass1;
        }
        else if (mass_cou == 3)
        {
            mass1 = ((mass1 + 360) + mass3) / 2;
        }
    }

    Serial.print("mass1,");
    Serial.print(mass1);
    Serial.print(" ");
    Serial.print("mass2,");
    Serial.print(mass2);
    Serial.print(" ");
    Serial.print("mass3,");
    Serial.print(mass3);
    Serial.print(" ");
    Serial.print("side,");
    Serial.print(side_line);
    Serial.print(" ");
    Serial.print("mode_line,");
    Serial.print(mode_line);
    Serial.print(" ");
    // Serial.print("mass_lat,");
    // Serial.print(mass_lat);
    // Serial.print(" ");
}

int line_dir_stop(int mass_1, int mass_2)
{
    float mass_sin = 0;
    float mass_cos = 0;
    int line_dir = 0;
    mass_sin = sin(radians(mass_1)) + sin(radians(mass_2));
    mass_cos = cos(radians(mass_1)) + cos(radians(mass_2));
    line_dir = atan2(mass_sin, mass_cos) * 180 / PI;
    return line_dir;
}
