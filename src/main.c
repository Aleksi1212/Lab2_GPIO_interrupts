#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/util/queue.h"

#include "../include/constants.h"
#include "../include/utils.h"

static queue_t queue;

void interrupt_callback(uint gpio, uint32_t events)
{
    if (gpio == ROT_A && (events & GPIO_IRQ_EDGE_RISE))
    {
        int rot_direction = 1;

        if (gpio_get(ROT_B) == 0) {
            queue_try_add(&queue, &rot_direction);
        } else {
            rot_direction = -1;
            queue_try_add(&queue, &rot_direction);
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

    queue_init(&queue, sizeof(int), 10);

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback);

    stdio_init_all();

    bool pressed = false;
    bool led_on = true;

    int rot_direction = 1;

    int led_level = MAX_LED_LEVEL;

    while (true)
    {
        while(queue_try_remove(&queue, &rot_direction)) {
            if (led_on)
                led_level += SCROLL_SPEED * rot_direction;
        }

        if (pressed && gpio_get(ROT_SW) == 0) {
            pressed = false;
        }
        else if (!pressed && gpio_get(ROT_SW) > 0) {
            if (led_on && led_level == 0)
                led_level = MAX_LED_LEVEL / 2;
            else
                led_on = !led_on;
            
            pressed = true;
            sleep_ms(10);
        }

        if (led_on) {
            if (led_level > MAX_LED_LEVEL)
                led_level = MAX_LED_LEVEL;
            if (led_level < 0)
                led_level = 0;

            adjust_leds(led_slices, led_level);
        }
        else
            adjust_leds(led_slices, 0);
    }

    return 0;
}
