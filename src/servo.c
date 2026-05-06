#include "servo.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

// Servo driver using a blocking 20ms pulse period on PC0.
// This avoids Timer2/MOSI conflict with the RFID/SPI hardware.
#define SERVO_PERIOD_US       20000u
#define SERVO_MIN_PULSE_US    1000u
#define SERVO_MAX_PULSE_US    2000u

static uint16_t servo_angle_to_pulse(uint8_t angle)
{
    if (angle > 180)
    {
        angle = 180;
    }

    return SERVO_MIN_PULSE_US + ((uint32_t)angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / 180;
}

static void servo_pulse(uint16_t pulse_us)
{
    uint16_t remaining = SERVO_PERIOD_US - pulse_us;

    SERVO_PORT |= (1U << SERVO_PIN);

    if (pulse_us > 1000u)
    {
        _delay_ms(1);
        _delay_us(pulse_us - 1000u);
    }
    else
    {
        _delay_us(pulse_us);
    }

    SERVO_PORT &= ~(1U << SERVO_PIN);

    while (remaining >= 1000u)
    {
        _delay_ms(1);
        remaining -= 1000u;
    }

    while (remaining > 0u)
    {
        _delay_us(1);
        remaining--;
    }
}

void servo_init(void)
{
    SERVO_DDR |= (1U << SERVO_PIN);
    SERVO_PORT &= ~(1U << SERVO_PIN);
}

void servo_open(void)
{
    uint16_t pulse = servo_angle_to_pulse(180);

    for (uint8_t i = 0; i < 20; i++)
    {
        servo_pulse(pulse);
    }
}

void servo_close(void)
{
    uint16_t pulse = servo_angle_to_pulse(0);

    for (uint8_t i = 0; i < 20; i++)
    {
        servo_pulse(pulse);
    }
}

void servo_set_angle(uint8_t angle)
{
    uint16_t pulse = servo_angle_to_pulse(angle);

    for (uint8_t i = 0; i < 20; i++)
    {
        servo_pulse(pulse);
    }
}