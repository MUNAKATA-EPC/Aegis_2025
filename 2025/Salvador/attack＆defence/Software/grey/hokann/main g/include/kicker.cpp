#include <Arduino.h>
#include "kicker.hpp"

void Kicker::init(int charge, int kick)
{
    this->_charge_pin = charge;
    this->_kick_pin = kick;

    pinMode(_charge_pin, OUTPUT);
    pinMode(_kick_pin, OUTPUT);
}

void Kicker::loop()
{
    _kick_timer.tick();

    if(_is_kicking)
    {
        Serial.println("kicker");

        if(_kick_timer.get_value() <= 20 + _start_delay_time)
        {
            digitalWrite(_charge_pin, 0);
            digitalWrite(_kick_pin, 0);
        }
        else if(_kick_timer.get_value() <= 100 + _start_delay_time)
        {
            digitalWrite(_charge_pin, 0);
            digitalWrite(_kick_pin, HIGH);
        }
        else if(_kick_timer.get_value() <= 150 + _start_delay_time)
        {
            digitalWrite(_charge_pin, 0);
            digitalWrite(_kick_pin, 0);
        }
        else if(_kick_timer.get_value() <= _delay_time + _start_delay_time)
        {
            digitalWrite(_charge_pin, HIGH);
            digitalWrite(_kick_pin, 0);
        }
        else
        {
            _kick_timer.reset();
            _is_kicking = false;
        }
    }
    else
    {
        digitalWrite(_kick_pin, 0);
        digitalWrite(_charge_pin, HIGH);
    }
}

void Kicker::kick(int delay, int start_delay)
{
    if(!_is_kicking)
    {
        _delay_time = delay;
        _start_delay_time = start_delay;
        _kick_timer.start();
        _is_kicking = true;
    }
}

bool Kicker::is_kicking()
{
    return this->_is_kicking;
}