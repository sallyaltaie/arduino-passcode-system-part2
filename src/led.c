#include "led.h"
#include "pins.h"
#include "shift_register.h"

// Track LED state
static uint8_t led_state = 0x00;

void led_init(void)
{
    // Shift register is already initialized by keypad_init
    // Just ensure LEDs are off
    led_state = 0x00;
}

void red_led_on(void)
{
    led_state |= (1U << LED_RED_BIT);
    shift_register_write(led_state);
}

void red_led_off(void)
{
    led_state &= ~(1U << LED_RED_BIT);
    shift_register_write(led_state);
}

void red_led_toggle(void)
{
    led_state ^= (1U << LED_RED_BIT);
    shift_register_write(led_state);
}

void green_led_on(void)
{
    led_state |= (1U << LED_GREEN_BIT);
    shift_register_write(led_state);
}

void green_led_off(void)
{
    led_state &= ~(1U << LED_GREEN_BIT);
    shift_register_write(led_state);
}

void green_led_toggle(void)
{
    led_state ^= (1U << LED_GREEN_BIT);
    shift_register_write(led_state);
}

void blue_led_on(void)
{
    led_state |= (1U << LED_BLUE_BIT);
    shift_register_write(led_state);
}

void blue_led_off(void)
{
    led_state &= ~(1U << LED_BLUE_BIT);
    shift_register_write(led_state);
}

void blue_led_toggle(void)
{
    led_state ^= (1U << LED_BLUE_BIT);
    shift_register_write(led_state);
}