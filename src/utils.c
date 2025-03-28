#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "../include/utils.h"
#include "../include/constants.h"

void init_rot_pin(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
}

void init_sw_pin(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

void init_led_pin(uint gpio, uint *slice_num)
{
	gpio_set_function(gpio, GPIO_FUNC_PWM);

	*slice_num = pwm_gpio_to_slice_num(gpio);

	pwm_set_clkdiv(*slice_num, CLOCK_DIVIDER);
	pwm_set_wrap(*slice_num, MAX_LED_LEVEL-1);
	pwm_set_enabled(*slice_num, true);
}

void adjust_leds(uint *led_slices, uint16_t level)
{
    for (int i = 0; i < LED_COUNT; i++) {
        pwm_set_both_levels(led_slices[i], level, level);
    }
}