#include "buzzer.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

#define BUZZER_FREQUENCY_HZ 1000u
#define BUZZER_DURATION_MS 500u

void buzzer_init(void)
{
    BUZZER_DDR |= (1U << BUZZER_PIN);
    BUZZER_PORT &= ~(1U << BUZZER_PIN);
}

void buzzer_scream(void)
{
    uint16_t half_period_us = 500000u / BUZZER_FREQUENCY_HZ;  // 500 for 1kHz
    uint16_t cycles = (BUZZER_DURATION_MS * BUZZER_FREQUENCY_HZ) / 1000u;

    for (uint16_t i = 0; i < cycles; i++)
    {
        BUZZER_PORT |= (1U << BUZZER_PIN);
        _delay_us(half_period_us);
        BUZZER_PORT &= ~(1U << BUZZER_PIN);
        _delay_us(half_period_us);
    }
}

void buzzer_quiet(void)
{
    BUZZER_PORT &= ~(1U << BUZZER_PIN);
}