#include "servo.h"
#include "pins.h"

#include <avr/io.h>

#define SERVO_PERIOD_TICKS     40000u
#define SERVO_0_DEG_TICKS       2000u
#define SERVO_180_DEG_TICKS     4000u

static uint16_t servo_angle_to_ticks(uint8_t angle)
{
    if (angle > 180U)
    {
        angle = 180U;
    }

    return SERVO_0_DEG_TICKS +
           ((uint32_t)angle * (SERVO_180_DEG_TICKS - SERVO_0_DEG_TICKS)) / 180U;
}

static void servo_set_pulse_ticks(uint16_t pulse_ticks)
{
    if (pulse_ticks < SERVO_0_DEG_TICKS)
    {
        pulse_ticks = SERVO_0_DEG_TICKS;
    }

    if (pulse_ticks > SERVO_180_DEG_TICKS)
    {
        pulse_ticks = SERVO_180_DEG_TICKS;
    }

    OCR1A = pulse_ticks;
}

void servo_init(void)
{
    // Timer1 PWM on OC1A (PB1 / Arduino D9).
    SERVO_DDR |= (1U << SERVO_PIN);
    SERVO_PORT &= ~(1U << SERVO_PIN);

    TCCR1A = 0;
    TCCR1B = 0;

    // Fast PWM, mode 14: TOP = ICR1.
    TCCR1A = (1U << WGM11) | (1U << COM1A1);
    TCCR1B = (1U << WGM12) | (1U << WGM13) | (1U << CS11);

    // 16 MHz / 8 prescaler = 2 MHz => 0.5 us per tick.
    ICR1 = SERVO_PERIOD_TICKS;
    servo_set_pulse_ticks(SERVO_180_DEG_TICKS);
}

void servo_open(void)
{
    servo_set_pulse_ticks(SERVO_0_DEG_TICKS);
}

void servo_close(void)
{
    servo_set_pulse_ticks(SERVO_180_DEG_TICKS);
}

void servo_set_angle(uint8_t angle)
{
    servo_set_pulse_ticks(servo_angle_to_ticks(angle));
}
