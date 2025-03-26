#include "pico/stdlib.h"

#ifndef UTILS_H
#define UTILS_H

void init_rot_pin(uint gpio);
void init_sw_pin(uint gpio);
void init_led_pin(uint gpio, uint *slice_num);
void adjust_leds(uint *led_slices, uint16_t level);

#endif