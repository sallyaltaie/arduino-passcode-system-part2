#include "millis.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile millis_t g_millis = 0;

void millis_init(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 16 MHz / 64 = 250 kHz, 250 counts = 1 ms
    OCR1A = 249;
    TCCR1B |= (1U << WGM12);
    TCCR1B |= (1U << CS11) | (1U << CS10);
    TIMSK1 |= (1U << OCIE1A);
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

ISR(TIMER1_COMPA_vect)
{
    g_millis++;
}
