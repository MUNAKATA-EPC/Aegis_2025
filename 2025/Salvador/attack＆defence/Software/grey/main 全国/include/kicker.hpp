#ifndef _KICKER_HPP
#define _KICKER_HPP

#include <timer.h>

class Kicker{
public:
    void init(int charge, int kick);
    void loop();
    void kick(int delay, int start_delay = 0);
    bool is_kicking();

private:
    int _delay_time;
    int _start_delay_time;
    int _charge_pin;
    int _kick_pin;
    bool _is_kicking;
    timer _kick_timer;
};

#endif