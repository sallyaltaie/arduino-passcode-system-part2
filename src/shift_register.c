#include "shift_register.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

// Shift register state (which bits are on/off)
static uint8_t shift_register_state = 0x00;

void shift_register_init(void)
{
    // Configure shift register pins as outputs
    SHIFT_REGISTER_DS_DDR |= (1U << SHIFT_REGISTER_DS_PIN);
    SHIFT_REGISTER_SHCP_DDR |= (1U << SHIFT_REGISTER_SHCP_PIN);
    SHIFT_REGISTER_STCP_DDR |= (1U << SHIFT_REGISTER_STCP_PIN);
    
    // Initialize all pins to low
    SHIFT_REGISTER_DS_PORT &= ~(1U << SHIFT_REGISTER_DS_PIN);
    SHIFT_REGISTER_SHCP_PORT &= ~(1U << SHIFT_REGISTER_SHCP_PIN);
    SHIFT_REGISTER_STCP_PORT &= ~(1U << SHIFT_REGISTER_STCP_PIN);
    
    // Clear the shift register
    shift_register_write(0x00);
}

void shift_register_write(uint8_t data)
{
    shift_register_state = data;
    
    // Shift out 8 bits, MSB first
    for (int8_t i = 7; i >= 0; i--)
    {
        // Set data line
        if (data & (1U << i))
        {
            SHIFT_REGISTER_DS_PORT |= (1U << SHIFT_REGISTER_DS_PIN);
        }
        else
        {
            SHIFT_REGISTER_DS_PORT &= ~(1U << SHIFT_REGISTER_DS_PIN);
        }
        
        _delay_us(1);
        
        // Pulse shift clock
        SHIFT_REGISTER_SHCP_PORT |= (1U << SHIFT_REGISTER_SHCP_PIN);
        _delay_us(1);
        SHIFT_REGISTER_SHCP_PORT &= ~(1U << SHIFT_REGISTER_SHCP_PIN);
        _delay_us(1);
    }
    
    // Pulse storage clock to latch the data
    SHIFT_REGISTER_STCP_PORT |= (1U << SHIFT_REGISTER_STCP_PIN);
    _delay_us(1);
    SHIFT_REGISTER_STCP_PORT &= ~(1U << SHIFT_REGISTER_STCP_PIN);
    _delay_us(1);
}

void shift_register_set_bit(uint8_t bit)
{
    if (bit < 8)
    {
        shift_register_write(shift_register_state | (1U << bit));
    }
}

void shift_register_clear_bit(uint8_t bit)
{
    if (bit < 8)
    {
        shift_register_write(shift_register_state & ~(1U << bit));
    }
}