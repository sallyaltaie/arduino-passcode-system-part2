#ifndef KEYPAD_H
#define KEYPAD_H

#include "shift_register.h"

void keypad_init(void);
char keypad_scan(void);

#endif
