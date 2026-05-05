#include "command.h"
#include "uart.h"
#include "pin_code.h"

#include <string.h>
#include <avr/pgmspace.h>

#define CMD_NEW_PIN_LENGTH 8

// Command prefix stored in flash memory
static const char CMD_NEW_PIN[] PROGMEM = "NEW PIN;";

// Process PIN change command: "NEW PIN;<old>;<new>"
uint8_t command_process(char *cmd)
{
    char *old_pin;
    char *new_pin;
    char *separator;

    // Check if command starts with "NEW PIN;"
    if (strncmp_P(cmd, CMD_NEW_PIN, CMD_NEW_PIN_LENGTH) != 0)
    {
        uart_write_string("Unknown command\n");
        return 0;
    }

    // Parse and search for OLD pin
    old_pin = cmd + CMD_NEW_PIN_LENGTH;
    separator = strchr(old_pin, ';');

    if (separator == 0)
    {
        uart_write_string("Usage: NEW PIN;<old pin>;<new pin>\n");
        return 0;
    }

    *separator = '\0';
    new_pin = separator + 1;

    // Validate PIN format (4 digits)
    if (!pin_code_is_valid(old_pin) || !pin_code_is_valid(new_pin))
    {
        uart_write_string("PIN must be 4 digits\n");
        return 0;
    }

    // Attempt to update PIN
    if (pin_code_update(old_pin, new_pin))
    {
        uart_write_string("PIN updated\n");
        return 1;
    }

    uart_write_string("Wrong old PIN\n");
    return 0;
}