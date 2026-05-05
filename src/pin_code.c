#include "pin_code.h"
#include "config.h"

#include <avr/eeprom.h>

#define PIN_EEPROM_MARKER 0x42U  // Marker to detect if PIN has been stored

static uint8_t EEMEM stored_marker;
static char EEMEM stored_pin[PIN_LENGTH];

static char current_pin[PIN_LENGTH];
static const char default_pin[PIN_LENGTH] = {'1', '7', '7', '2'};

static void copy_pin(char *destination, const char *source)
{
    uint8_t i;

    for (i = 0; i < PIN_LENGTH; i++)
    {
        destination[i] = source[i];
    }
}

static void save_current_pin(void)
{
    eeprom_update_block(current_pin, stored_pin, PIN_LENGTH);
    eeprom_update_byte(&stored_marker, PIN_EEPROM_MARKER);
}

static void use_default_pin(void)
{
    copy_pin(current_pin, default_pin);
    save_current_pin();
}

static uint8_t pin_has_four_digits(const char *pin)
{
    uint8_t i;

    for (i = 0; i < PIN_LENGTH; i++)
    {
        if (pin[i] < '0' || pin[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

void pin_code_init(void)
{
    // Check if PIN has been stored in EEPROM before
    if (eeprom_read_byte(&stored_marker) != PIN_EEPROM_MARKER)
    {
        use_default_pin();
        return;
    }

    eeprom_read_block(current_pin, stored_pin, PIN_LENGTH);

    // Validate stored PIN, use default if invalid
    if (!pin_has_four_digits(current_pin))
    {
        use_default_pin();
    }
}

uint8_t pin_code_is_valid(const char *pin)
{
    return pin_has_four_digits(pin) && pin[PIN_LENGTH] == '\0';
}

uint8_t pin_code_matches(const char *pin)
{
    uint8_t i;

    for (i = 0; i < PIN_LENGTH; i++)
    {
        if (pin[i] != current_pin[i])
        {
            return 0;
        }
    }

    return 1;
}

uint8_t pin_code_update(const char *old_pin, const char *new_pin)
{
    // Validate both PINs are 4 digits and null-terminated
    if (!pin_code_is_valid(old_pin) || !pin_code_is_valid(new_pin))
    {
        return 0;
    }

    // Check if old PIN matches current PIN
    if (!pin_code_matches(old_pin))
    {
        return 0;
    }

    // Update current PIN and save to EEPROM
    copy_pin(current_pin, new_pin);
    save_current_pin();

    return 1;
}
