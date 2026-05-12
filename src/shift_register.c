#include "shift_register.h"
#include "pins.h"
#include "spi.h"

#include <avr/io.h>
#include <util/delay.h>

static uint8_t shift_register_state = 0x00;

static void shift_register_latch(void)
{
    LATCH_PORT |= (1U << LATCH_PIN);
    _delay_us(5);
    LATCH_PORT &= ~(1U << LATCH_PIN);
}

void shift_register_init(void)
{
    LATCH_DDR |= (1U << LATCH_PIN);
    LATCH_PORT &= ~(1U << LATCH_PIN);
    shift_register_write(0x00);
}

void shift_register_write(uint8_t data)
{
    shift_register_state = data;
    spi_transfer(data);
    shift_register_latch();
}

uint8_t shift_register_get_state(void)
{
    return shift_register_state;
}

void shift_register_set_bit(uint8_t bit)
{
    if (bit < 8U)
    {
        shift_register_write(shift_register_state | (1U << bit));
    }
}

void shift_register_clear_bit(uint8_t bit)
{
    if (bit < 8U)
    {
        shift_register_write(shift_register_state & (uint8_t)~(1U << bit));
    }
}
