#include "led.h"
#include "pins.h"

void led_init(void)
{
    RED_LED_DDR |= (1U << RED_LED_PIN);
    GREEN_LED_DDR |= (1U << GREEN_LED_PIN);

    RED_LED_PORT &= ~(1U << RED_LED_PIN);
    GREEN_LED_PORT &= ~(1U << GREEN_LED_PIN);
}

void red_led_on(void)
{
    RED_LED_PORT |= (1U << RED_LED_PIN);
}

void red_led_off(void)
{
    RED_LED_PORT &= ~(1U << RED_LED_PIN);
}

void red_led_toggle(void)
{
    RED_LED_PORT ^= (1U << RED_LED_PIN);
}

void green_led_on(void)
{
    GREEN_LED_PORT |= (1U << GREEN_LED_PIN);
}

void green_led_off(void)
{
    GREEN_LED_PORT &= ~(1U << GREEN_LED_PIN);
}

void green_led_toggle(void)
{
    GREEN_LED_PORT ^= (1U << GREEN_LED_PIN);
}
