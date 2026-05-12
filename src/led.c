#include "led.h"
#include "pins.h"
#include "shift_register.h"

void led_init(void)
{
    red_led_off();
    green_led_off();
    blue_led_off();
}

void red_led_on(void)
{
    shift_register_set_bit(LED_RED_BIT);
}

void red_led_off(void)
{
    shift_register_clear_bit(LED_RED_BIT);
}

void red_led_toggle(void)
{
    if (shift_register_get_state() & (1U << LED_RED_BIT))
    {
        shift_register_clear_bit(LED_RED_BIT);
    }
    else
    {
        shift_register_set_bit(LED_RED_BIT);
    }
}

void green_led_on(void)
{
    shift_register_set_bit(LED_GREEN_BIT);
}

void green_led_off(void)
{
    shift_register_clear_bit(LED_GREEN_BIT);
}

void green_led_toggle(void)
{
    if (shift_register_get_state() & (1U << LED_GREEN_BIT))
    {
        shift_register_clear_bit(LED_GREEN_BIT);
    }
    else
    {
        shift_register_set_bit(LED_GREEN_BIT);
    }
}

void blue_led_on(void)
{
    shift_register_set_bit(LED_BLUE_BIT);
}

void blue_led_off(void)
{
    shift_register_clear_bit(LED_BLUE_BIT);
}

void blue_led_toggle(void)
{
    if (shift_register_get_state() & (1U << LED_BLUE_BIT))
    {
        shift_register_clear_bit(LED_BLUE_BIT);
    }
    else
    {
        shift_register_set_bit(LED_BLUE_BIT);
    }
}
