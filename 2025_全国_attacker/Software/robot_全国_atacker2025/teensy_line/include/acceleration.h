
#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <gyro.h>

float v;
float a_x;
float a_y;
float a_x_old;
float a_y_old;
float v_x;
float v_y;
float a_xy;
float v_deg;

void acceleration()
{
    imu::Vector<3> accelermetor = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    a_x = accelermetor.x();
    a_y = accelermetor.y();
    a_x = a_x*100;
    a_y = a_y*100;

    a_xy = sqrt(pow(a_x,2) + pow(a_y,2));
    v_x = a_x + a_x_old;
    v_y = a_y + a_y_old;
    a_x_old  = a_x;
    a_y_old  = a_y;
    v = sqrt(pow(v_x,2) + pow(v_y,2));
    v -=135;
    v = v_ave.moving_average(v);
    // v_deg = atan2(v_x,v_y) * 180 / PI;
    // v_deg -=82;
    // v_deg = v_deg_ave.moving_average(v_deg);

}
