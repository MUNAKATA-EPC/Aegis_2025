#include <Arduino.h>

#ifndef _TIMER_H
#define _TIMER_H

class timer
{
private:
    int _start_val;
    int value;
    bool ticking;

public:
    void tick()
    {
        if (ticking){
            value = millis() - _start_val;
        }
    }
    void start()
    {   
        if (ticking == false)
        {
            _start_val = millis();
        }

        ticking = true;
        
        
    }
    void stop()
    {
        ticking = false;
    }

    void reset()
    {
        ticking = false;
        value = 0;
    }
   int get_value()
    {
        return value;
    }
    /*float get_value()
    {
        return value;
    }*/
    bool isticking()
    {
        return ticking;
    }
};

timer LINE_reaction;
timer KICK_TEST;
timer LINE_deep;
timer STARTkick;
timer KICKOFF;
timer KICK;
timer KICK_camera;
timer DRIBBLER;
timer OLED;
timer BUTTON_left;
timer BUTTON_right;
timer BUTTON_center;
timer RULU;
timer LINE;

#endif