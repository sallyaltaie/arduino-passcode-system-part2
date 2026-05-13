#include "millis.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile millis_t g_millis = 0;

void millis_init(void)
{
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    // 16 MHz / 64 = 250 kHz, 250 counts = 1 ms
    OCR0A = 249;
    TCCR0A |= (1U << WGM01);
    TCCR0B |= (1U << CS01) | (1U << CS00);
    TIMSK0 |= (1U << OCIE0A);
}

// Add time
millis_t millis_get(void)
{
    millis_t value;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        value = g_millis;
    }

    return value;
}

ISR(TIMER0_COMPA_vect)
{
    g_millis++;
}
