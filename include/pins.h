#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

#define GREEN_LED_DDR   DDRD
#define GREEN_LED_PORT  PORTD
#define GREEN_LED_PIN   PD2

#define RED_LED_DDR     DDRD
#define RED_LED_PORT    PORTD
#define RED_LED_PIN     PD3

#define GREEN_BUTTON_DDR   DDRB
#define GREEN_BUTTON_PORT  PORTB
#define GREEN_BUTTON_PIN   PB4

#define KEYPAD_ROW_DDR     DDRB
#define KEYPAD_ROW_PORT    PORTB
#define KEYPAD_ROW_PINREG  PINB
#define KEYPAD_R1          PB3
#define KEYPAD_R2          PB2
#define KEYPAD_R3          PB1
#define KEYPAD_R4          PB0

#define KEYPAD_COL_DDR     DDRD
#define KEYPAD_COL_PORT    PORTD
#define KEYPAD_COL_PINREG  PIND
#define KEYPAD_C1          PD7
#define KEYPAD_C2          PD6
#define KEYPAD_C3          PD5
#define KEYPAD_C4          PD4

#endif
