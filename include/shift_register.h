#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <stdint.h>

void shift_register_init(void);
void shift_register_write(uint8_t data);
uint8_t shift_register_get_state(void);
void shift_register_set_bit(uint8_t bit);
void shift_register_clear_bit(uint8_t bit);

#endif
