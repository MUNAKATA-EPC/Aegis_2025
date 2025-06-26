#include <Arduino.h>
#include <Servo.h>

#define mode_changepin 3

int mode_change;

Servo dribbler;

void setup()
{
    dribbler.attach(9);
    pinMode(mode_changepin, INPUT_PULLDOWN);

    delay(100);
    dribbler.writeMicroseconds(2000);
    delay(100);
    dribbler.writeMicroseconds(1000);
    delay(10000);
}

void loop()
{
    mode_change = digitalRead(mode_changepin);
    if (mode_change == 1)
    {
        dribbler.writeMicroseconds(1200);
    }
    else
    {
        dribbler.writeMicroseconds(100);
    }
}