#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

// Legacy direct LED pins from the earlier assignment stage.
// The current RGB LED is driven through the 74HC595 instead.
#define GREEN_LED_DDR   DDRD
#define GREEN_LED_PORT  PORTD
#define GREEN_LED_PIN   PD2

#define RED_LED_DDR     DDRD
#define RED_LED_PORT    PORTD
#define RED_LED_PIN     PD3

#define GREEN_BUTTON_DDR     DDRC
#define GREEN_BUTTON_PORT    PORTC
#define GREEN_BUTTON_PINREG  PINC
#define GREEN_BUTTON_PIN     PC0

#define KEYPAD_COL_DDR     DDRD
#define KEYPAD_COL_PORT    PORTD
#define KEYPAD_COL_PINREG  PIND
#define KEYPAD_C1          PD7
#define KEYPAD_C2          PD6
#define KEYPAD_C3          PD5
#define KEYPAD_C4          PD4
 
// Shift Register pins (74HC595)
#define SHIFT_REGISTER_DS_DDR   DDRB
#define SHIFT_REGISTER_DS_PORT  PORTB
#define SHIFT_REGISTER_DS_PIN   PB3

#define SHIFT_REGISTER_SHCP_DDR   DDRB
#define SHIFT_REGISTER_SHCP_PORT  PORTB
#define SHIFT_REGISTER_SHCP_PIN   PB5

#define SHIFT_REGISTER_STCP_DDR   DDRD
#define SHIFT_REGISTER_STCP_PORT  PORTD
#define SHIFT_REGISTER_STCP_PIN   PD2

// LED bits in shift register 
#define LED_RED_BIT 4
#define LED_GREEN_BIT 5
#define LED_BLUE_BIT 6

// Servo control pin
#define SERVO_DDR    DDRB
#define SERVO_PORT   PORTB
#define SERVO_PIN    PB1  // D9 on Arduino Uno

// Buzzer control pin
#define BUZZER_DDR   DDRD
#define BUZZER_PORT  PORTD
#define BUZZER_PIN   PD3  // D3 on Arduino Uno

// MFRC522 control pins
#define MFRC522_CS_DDR    DDRB
#define MFRC522_CS_PORT   PORTB
#define MFRC522_CS_PIN    PB2   // D10 / SS

#define MFRC522_RST_DDR   DDRB
#define MFRC522_RST_PORT  PORTB
#define MFRC522_RST_PIN   PB0   // D8

#endif
