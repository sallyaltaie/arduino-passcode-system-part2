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

#define KEYPAD_COL_DDR     DDRD
#define KEYPAD_COL_PORT    PORTD
#define KEYPAD_COL_PINREG  PIND
#define KEYPAD_C1          PD7
#define KEYPAD_C2          PD6
#define KEYPAD_C3          PD5
#define KEYPAD_C4          PD4
 
// Shift Register pins (74HC595)
#define SHIFT_REGISTER_DS_DDR   DDRD
#define SHIFT_REGISTER_DS_PORT  PORTD
#define SHIFT_REGISTER_DS_PIN   PD0

#define SHIFT_REGISTER_SHCP_DDR   DDRD
#define SHIFT_REGISTER_SHCP_PORT  PORTD
#define SHIFT_REGISTER_SHCP_PIN   PD1

#define SHIFT_REGISTER_STCP_DDR   DDRB
#define SHIFT_REGISTER_STCP_PORT  PORTB
#define SHIFT_REGISTER_STCP_PIN   PB5

// LED bits in shift register 
#define LED_RED_BIT 4
#define LED_GREEN_BIT 5
#define LED_BLUE_BIT 6

#endif
