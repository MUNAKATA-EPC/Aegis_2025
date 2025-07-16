#include <Arduino.h>

#ifndef _ENGELLINE_HPP
#define _ENGELLINE_HPP

extern void init_engelline();
extern void process_engelline(bool running);

extern bool is_line_detected();
extern bool is_line_evacuation();
extern bool is_previous_line_detected();
extern bool is_exist_deg_value_in_range(int check_value, int base_value, int check_range);

extern int line_deg;
extern int line_first_deg;
extern bool is_halfout;
extern int line_evacuation_deg;

extern int line_data[16];
#endif