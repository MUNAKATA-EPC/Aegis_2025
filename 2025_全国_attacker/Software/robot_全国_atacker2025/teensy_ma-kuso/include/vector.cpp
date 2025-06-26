#include <Arduino.h>
#include "vector.hpp"

Vector::Vector(float rad, float size)
{
    this->_rad = rad;//このクラスのrad
    this->_size = size;//このクラスのsize
}

void Vector::add(Vector a)
{
    float origin_vec_x = cos(this->_rad) * this->_size;
    float origin_vec_y = sin(this->_rad) * this->_size;
    float add_vec_x = cos(a._rad) * a._size;
    float add_vec_y = sin(a._rad) * a._size;

    this->_rad = atan2(origin_vec_y + add_vec_y, origin_vec_x + add_vec_x);//ベクトルの角度
    this->_size = sqrt(pow(origin_vec_x + add_vec_x, 2) + pow(origin_vec_y + add_vec_y, 2));//ベクトルの大きさ
}

void Vector::sub(Vector a)
{
    float origin_vec_x = cos(this->_rad) * this->_size;
    float origin_vec_y = sin(this->_rad) * this->_size;
    float add_vec_x = cos(a._rad) * a._size;
    float add_vec_y = sin(a._rad) * a._size;

    this->_rad = atan2(origin_vec_y - add_vec_y, origin_vec_x - add_vec_x);
    this->_size = sqrt(pow(origin_vec_x - add_vec_x, 2) + pow(origin_vec_y - add_vec_y, 2));
}

int Vector::get_deg()
{
    return (int)degrees(_rad);
}

float Vector::get_size()
{
    return _size;
}

Vector Vector::get_x_component()
{
    float ret_size = cos(this->_rad) * this->_size;
    return Vector(ret_size >= 0 ? cos(0) : cos(PI), ret_size);
}

Vector Vector::get_y_component()
{
    float ret_size = sin(this->_rad) * this->_size;
    return Vector(ret_size >= 0 ? sin(0) : sin(PI), ret_size);
}