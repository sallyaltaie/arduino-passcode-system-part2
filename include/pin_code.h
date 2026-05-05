#ifndef PIN_CODE_H
#define PIN_CODE_H

#include <stdint.h>

void pin_code_init(void);
uint8_t pin_code_matches(const char *pin);
uint8_t pin_code_is_valid(const char *pin);
uint8_t pin_code_update(const char *old_pin, const char *new_pin);

#endif
