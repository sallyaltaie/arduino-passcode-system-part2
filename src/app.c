#include "app.h"
#include "app_console.h"
#include "app_rfid.h"
#include "config.h"
#include "led.h"
#include "pins.h"
#include "uart.h"
#include "millis.h"
#include "keypad.h"
#include "pin_code.h"
#include "servo.h"
#include "spi.h"
#include "i2c.h"
#include "ds1307.h"
#include "buzzer.h"

#include <stdio.h>

#define RED_BLINK_INTERVAL_MS 500UL
#define KEY_FEEDBACK_MS 100UL
#define BLUE_RFID_BLINK_MS 150UL
#define BUZZER_ERROR_MS 300UL

// State machine states for PIN access system
typedef enum {
    STATE_IDLE,             // Red LED on, waiting for green button press
    STATE_INPUT_AWAIT,      // Red LED blinking, waiting for PIN input
    STATE_ACCESS_GRANTED    // Green LED on, access granted for 3 seconds
} app_state_t;

// Variables
static app_state_t current_state;
static char entered_pin[4];
static uint8_t entered_length;
static millis_t state_start_time;
static millis_t red_blink_time;
static millis_t key_feedback_time;
static millis_t blue_blink_time;
static millis_t buzzer_error_time;
static millis_t rtc_print_time;
static uint8_t key_feedback_active;
static uint8_t blue_blink_active;
static uint8_t buzzer_error_active;

// Helper functions for PIN input handling
static void reset_input(void);
static void save_key(char key);
static uint8_t pin_is_complete(void);
static uint8_t input_timeout_expired(void);
static uint8_t access_time_expired(void);
static void update_red_blink(void);
static void blink_key_feedback(void);
static void update_key_feedback(void);
static void update_blue_blink(void);
static void buzz_error_once(void);
static void update_buzzer_error(void);
static void print_rtc_time(void);

static void print_rtc_time(void)
{
    DateTime now;
    char buffer[32];

    ds1307_get_datetime(&now);

    snprintf(buffer, sizeof(buffer), "Time: %02u:%02u:%02u\n",
             now.hour, now.minute, now.second);
    uart_write_string(buffer);
}

static void reset_input(void)
{
    entered_length = 0;
}

static void save_key(char key)
{
    if (entered_length < PIN_LENGTH)
    {
        entered_pin[entered_length] = key;
        entered_length++;
    }
}

static uint8_t pin_is_complete(void)
{
    return entered_length == PIN_LENGTH;
}

static uint8_t input_timeout_expired(void)
{
    return (millis() - state_start_time) >= INPUT_TIMEOUT_MS;
}

static uint8_t access_time_expired(void)
{
    return (millis() - state_start_time) >= ACCESS_GRANTED_MS;
}

// Change application state and update LED status accordingly
static void set_state(app_state_t new_state)
{
    current_state = new_state;
    state_start_time = millis();

    if (new_state == STATE_IDLE)
    {
        reset_input();
        key_feedback_active = 0;
        blue_blink_active = 0;
        red_led_on();
        green_led_off();
        blue_led_off();
        servo_close();
    }
    else if (new_state == STATE_INPUT_AWAIT)
    {
        red_blink_time = millis();
        key_feedback_active = 0;
        blue_blink_active = 0;
        red_led_on();
        green_led_off();
        blue_led_off();
    }
    else if (new_state == STATE_ACCESS_GRANTED)
    {
        key_feedback_active = 0;
        blue_blink_active = 0;
        buzzer_error_active = 0;
        red_led_off();
        green_led_on();
        blue_led_off();
        buzzer_quiet();
        servo_open();
    }
}
static void update_red_blink(void)
{
    if ((millis() - red_blink_time) >= RED_BLINK_INTERVAL_MS)
    {
        red_led_toggle();
        red_blink_time = millis();
    }
}

static void blink_key_feedback(void)
{
    red_led_on();
    green_led_on();
    key_feedback_time = millis();
    key_feedback_active = 1;
}

static void update_key_feedback(void)
{
    if (key_feedback_active && ((millis() - key_feedback_time) >= KEY_FEEDBACK_MS))
    {
        green_led_off();
        key_feedback_active = 0;
        red_blink_time = millis();
    }
}

static void update_blue_blink(void)
{
    if (blue_blink_active && ((millis() - blue_blink_time) >= BLUE_RFID_BLINK_MS))
    {
        blue_led_off();
        blue_blink_active = 0;
    }
}

static void buzz_error_once(void)
{
    buzzer_scream();
    buzzer_error_time = millis();
    buzzer_error_active = 1;
}

static void update_buzzer_error(void)
{
    if (buzzer_error_active && ((millis() - buzzer_error_time) >= BUZZER_ERROR_MS))
    {
        buzzer_quiet();
        buzzer_error_active = 0;
    }
}

static uint8_t pin_is_correct(void)
{
    return pin_code_matches(entered_pin);
}

void app_init(void)
{
    DateTime initial_time = {0, 0, 12, 1, 1, 1, 24};

    // Green start button as input with pull-up
    GREEN_BUTTON_DDR &= ~(1U << GREEN_BUTTON_PIN);
    GREEN_BUTTON_PORT |= (1U << GREEN_BUTTON_PIN);

    // timer
    millis_init();

    uart_init(UART_BAUDRATE);
    app_console_init();

    // SPI must be initialized before the shift register is used by keypad/LED.
    spi_init();

    // keypad + shift register
    keypad_init();
    led_init();

    // RFID
    app_rfid_init();

    // RTC
    twi_init(100000UL);
    ds1307_set_datetime(&initial_time);
    ds1307_start_clock();

    // Buzzer
    buzzer_init();

    // Servo
    servo_init();

    // PIN
    pin_code_init();

    // start the system in IDLE
    set_state(STATE_IDLE);
    rtc_print_time = millis();
    print_rtc_time();

    uart_write_string("System ready\n");
    uart_write_string("Type: help\n");
}

// Main application loop - handles state machine
void app_run(void)
{
    app_console_handle(1);

    if (!app_console_has_pending_input() &&
        ((millis() - rtc_print_time) >= 1000UL))
    {
        rtc_print_time = millis();
        print_rtc_time();
    }

    switch (current_state)
    {
        case STATE_IDLE:
            if (app_rfid_check())
            {
                set_state(STATE_ACCESS_GRANTED);
            }
            else
            {
                update_blue_blink();
                update_buzzer_error();
                set_state(STATE_INPUT_AWAIT);
            }
            break;

        case STATE_INPUT_AWAIT:
        {
            char key;

            if (app_rfid_check())
            {
                set_state(STATE_ACCESS_GRANTED);
                break;
            }
            update_blue_blink();
            update_buzzer_error();
            if (key_feedback_active)
            {
                update_key_feedback();
            }
            else
            {
                update_red_blink();
            }

            if (input_timeout_expired())
            {
                set_state(STATE_IDLE);
            }
            else
            {
                key = keypad_scan();

                if (key != '\0')
                {
                    blink_key_feedback();
                    uart_write_string("Key: ");
                    uart_write_char(key);
                    uart_write_string("\n");
                    save_key(key);
                    if (pin_is_complete())
                    {
                        if (pin_is_correct())
                        {
                            set_state(STATE_ACCESS_GRANTED);
                        }
                        else
                        {
                            uart_write_string("Wrong PIN\n");
                            buzz_error_once();
                            set_state(STATE_IDLE);
                        }
                    }
                }
            }
            break;
        }

        case STATE_ACCESS_GRANTED:
            // Button and keypad input are ignored while access is granted.
            if (access_time_expired())
            {
                green_led_off();
                set_state(STATE_IDLE);
            }
            break;

        default:
            set_state(STATE_IDLE);
            break;
    }
}
