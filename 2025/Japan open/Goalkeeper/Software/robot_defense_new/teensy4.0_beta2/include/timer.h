#include <Arduino.h>

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

timer LINE;
timer IR_Ball;
timer OLED;
timer BUTTON_left;
timer BUTTON_right;
timer BUTTON_center;
timer RULU;
//timer CAMERA;
//timer SPEED;
timer KICK;
timer KICK_TEST;