
#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <SPI.h>

#include <gyro.h>


int a_x;
int a_y;
int a_x_old;
int a_y_old;

int v_x;
int v_y;
int v_speed;
int a_xy;

int v;





int acceleration()
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

}
