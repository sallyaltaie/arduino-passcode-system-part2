#include "app.h"
#include <avr/interrupt.h>

// Main entry point for PIN access system
int main(void)
{
    app_init();
    sei();  // Enable global interrupts

    // Main application loop
    while (1)
    {
        // shift_register_set_bits(0b10000000);
        // _delay_ms(250);
        // shift_register_clear_bits(0b10000000);
        // shift_register_set_bits(0b01000000);
        // _delay_ms(250);
        // shift_register_clear_bits(0b01000000);
        // shift_register_set_bits(0b00100000);
        // _delay_ms(250);
        // shift_register_clear_bits(0b00100000);
        app_run();
    }
}
