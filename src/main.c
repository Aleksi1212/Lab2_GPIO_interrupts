#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "../include/constants.h"
#include "../include/utils.h"

static bool led_on = true;
static uint16_t led_level = MAX_LED_LEVEL + 1;

void change_led_level(uint gpio, uint32_t events)
{
    if (led_on) {
        int direction = gpio_get(ROT_B);
        if (direction == 0 && led_level < MAX_LED_LEVEL + 1) {
            led_level += 50;
        } else if (direction > 0 && led_level > 0) {
            led_level -= 50;
        }
    }
}

int main(void)
{
    uint led_slices[LED_COUNT];

    init_led_pin(LED_1, &led_slices[0]);
    init_led_pin(LED_2, &led_slices[1]);
    init_led_pin(LED_3, &led_slices[2]);

    init_rot_pin(ROT_A);
    init_rot_pin(ROT_B);

    init_sw_pin(ROT_SW);

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, &change_led_level);

    stdio_init_all();

    bool pressed = false;

    while (true)
    {
        if (pressed && gpio_get(ROT_SW) == 0) {
            pressed = false;
        }
        else if (!pressed && gpio_get(ROT_SW) > 0) {
            if (led_on && led_level == 0)
                led_level = (MAX_LED_LEVEL + 1) / 2;
            else
                led_on = !led_on;
            
            pressed = true;
            sleep_ms(10);
        }

        if (led_on)
            adjust_leds(led_slices, led_level);
        else
            adjust_leds(led_slices, 0);
    }

    return 0;
}
