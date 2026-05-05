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
        app_run();
    }
}
