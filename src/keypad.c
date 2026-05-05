#include "keypad.h"
#include "pins.h"

#include <stdint.h>
#include <util/delay.h>

// 4x4 keypad layout mapping
static const char keypad_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static void keypad_rowsHigh(void)
{
    KEYPAD_ROW_PORT |= (1U << KEYPAD_R1) |
                       (1U << KEYPAD_R2) |
                       (1U << KEYPAD_R3) |
                       (1U << KEYPAD_R4);
}

static void keypad_rowLow(uint8_t row)
{
    keypad_rowsHigh();

    switch (row)
    {
        case 0:
            KEYPAD_ROW_PORT &= ~(1U << KEYPAD_R1);
            break;
        case 1:
            KEYPAD_ROW_PORT &= ~(1U << KEYPAD_R2);
            break;
        case 2:
            KEYPAD_ROW_PORT &= ~(1U << KEYPAD_R3);
            break;
        case 3:
            KEYPAD_ROW_PORT &= ~(1U << KEYPAD_R4);
            break;
        default:
            break;
    }
}

static uint8_t keypad_readColumn(void)
{
    if ((KEYPAD_COL_PINREG & (1U << KEYPAD_C1)) == 0U) return 0U;
    if ((KEYPAD_COL_PINREG & (1U << KEYPAD_C2)) == 0U) return 1U;
    if ((KEYPAD_COL_PINREG & (1U << KEYPAD_C3)) == 0U) return 2U;
    if ((KEYPAD_COL_PINREG & (1U << KEYPAD_C4)) == 0U) return 3U;

    return 0xFFU;
}

static char keypad_getKey(void)
{
    uint8_t row;
    uint8_t column;

    for (row = 0U; row < 4U; row++)
    {
        keypad_rowLow(row);
        _delay_us(5);

        column = keypad_readColumn();
        if (column != 0xFFU)
        {
            keypad_rowsHigh();
            return keypad_map[row][column];
        }
    }

    keypad_rowsHigh();
    return '\0';
}

void keypad_init(void)
{
    // Configure rows as outputs, initially high
    KEYPAD_ROW_DDR |= (1U << KEYPAD_R1) |
                      (1U << KEYPAD_R2) |
                      (1U << KEYPAD_R3) |
                      (1U << KEYPAD_R4);

    keypad_rowsHigh();

    // Configure columns as inputs with pull-ups
    KEYPAD_COL_DDR &= ~((1U << KEYPAD_C1) |
                        (1U << KEYPAD_C2) |
                        (1U << KEYPAD_C3) |
                        (1U << KEYPAD_C4));

    KEYPAD_COL_PORT |= (1U << KEYPAD_C1) |
                       (1U << KEYPAD_C2) |
                       (1U << KEYPAD_C3) |
                       (1U << KEYPAD_C4);
}

// Scan keypad and return pressed key, with debouncing
char keypad_scan(void)
{
    static char last_key = '\0';
    char key = keypad_getKey();

    if (key == '\0')
    {
        last_key = '\0';
        return '\0';
    }

    if (key == last_key)
    {
        return '\0';
    }

    _delay_ms(20);  // Debounce delay

    if (key != keypad_getKey())
    {
        return '\0';
    }

    last_key = key;
    return key;
}
