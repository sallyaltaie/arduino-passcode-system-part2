#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <stdint.h>

// Initialize shift register control pins
void shift_register_init(void);

// Set a bit in the shift register (0-7)
void shift_register_set_bit(uint8_t bit);

// Clear a bit in the shift register (0-7)
void shift_register_clear_bit(uint8_t bit);

// Write all bits to the shift register
void shift_register_write(uint8_t data);

#endif