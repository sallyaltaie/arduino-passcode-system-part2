#ifndef APP_CONSOLE_H
#define APP_CONSOLE_H

#include <stdint.h>

void app_console_init(void);
void app_console_handle(uint8_t commands_allowed);
uint8_t app_console_has_pending_input(void);

#endif
