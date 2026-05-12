#include "app.h"
#include "app_console.h"
#include "config.h"
#include "led.h"
#include "pins.h"
#include "uart.h"
#include "millis.h"
#include "keypad.h"
#include "pin_code.h"
#include "spi.h"
#include "mfrc522.h"
#include "rtc.h"
#include "buzzer.h"

#define RED_BLINK_INTERVAL_MS 500UL
#define GREEN_KEY_BLINK_MS 100UL
#define BLUE_RFID_BLINK_MS 150UL
#define RFID_COOLDOWN_MS 1000UL

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
static millis_t green_blink_time;
static millis_t blue_blink_time;
static millis_t last_rfid_read_time;
static uint8_t green_blink_active;
static uint8_t blue_blink_active;

// Helper functions for PIN input handling
static void reset_input(void);
static void save_key(char key);
static uint8_t pin_is_complete(void);
static uint8_t input_timeout_expired(void);
static uint8_t access_time_expired(void);
static void update_red_blink(void);
static void blink_green_once(void);
static void update_green_blink(void);
static void blink_blue_once(void);
static void update_blue_blink(void);
static void app_rfid_check(void);

static uint8_t green_button_pressed(void)
{
    return (GREEN_BUTTON_PINREG & (1U << GREEN_BUTTON_PIN)) == 0;
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
        green_blink_active = 0;
        blue_blink_active = 0;
        red_led_on();
        green_led_off();
        blue_led_off();
    }
    else if (new_state == STATE_INPUT_AWAIT)
    {
        red_blink_time = millis();
        green_blink_active = 0;
        blue_blink_active = 0;
        red_led_on();
        green_led_off();
        blue_led_off();
    }
    else if (new_state == STATE_ACCESS_GRANTED)
    {
        green_blink_active = 0;
        blue_blink_active = 0;
        red_led_off();
        green_led_on();
        blue_led_off();
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

static void blink_green_once(void)
{
    green_led_on();
    green_blink_time = millis();
    green_blink_active = 1;
}

static void update_green_blink(void)
{
    if (green_blink_active && ((millis() - green_blink_time) >= GREEN_KEY_BLINK_MS))
    {
        green_led_off();
        green_blink_active = 0;
    }
}

static void blink_blue_once(void)
{
    blue_led_on();
    blue_blink_time = millis();
    blue_blink_active = 1;
}

static void update_blue_blink(void)
{
    if (blue_blink_active && ((millis() - blue_blink_time) >= BLUE_RFID_BLINK_MS))
    {
        blue_led_off();
        blue_blink_active = 0;
    }
}

static void app_rfid_check(void)
{
    uint8_t atqa[2];
    uint8_t atqa_len = 0;
    mfrc522_uid_t uid;
    mfrc522_status_t status;

    if ((millis() - last_rfid_read_time) < RFID_COOLDOWN_MS)
    {
        return;
    }

    // First, ask the reader if a tag is nearby
    status = mfrc522_request_a(atqa, &atqa_len);
    if (status != MFRC522_OK)
    {
        return; // no tag was found
    }

    // Next, read the tag's unique ID (UID)
    status = mfrc522_anticoll_select(&uid);
    if (status != MFRC522_OK)
    {
        return; // the tag could not be read
    }

    // Then, send the UID to the serial monitor
    uart_write_string("RFID UID: ");
    for (uint8_t i = 0; i < uid.size; i++)
    {
        uint8_t value = uid.uid[i];
        char buffer[3];
        buffer[0] = "0123456789ABCDEF"[(value >> 4) & 0x0F];
        buffer[1] = "0123456789ABCDEF"[value & 0x0F];
        buffer[2] = '\0';
        uart_write_string(buffer);
        if (i < uid.size - 1)
        {
            uart_write_string(" ");
        }
    }
    uart_write_string("\n");

    last_rfid_read_time = millis();
    blink_blue_once();

    // Finally, tell the tag to stop sending data
    mfrc522_halt();
}

static uint8_t pin_is_correct(void)
{
    return pin_code_matches(entered_pin);
}

void app_init(void)
{
    // Green start button as input with pull-up
    GREEN_BUTTON_DDR &= ~(1U << GREEN_BUTTON_PIN);
    GREEN_BUTTON_PORT |= (1U << GREEN_BUTTON_PIN);

    // timer
    millis_init();

    // SPI must be initialized before the shift register is used by keypad/LED.
    spi_init();

    // keypad + shift register
    keypad_init();
    led_init();

    // RFID
    mfrc522_init();

    // RTC
    rtc_init();

    // Buzzer
    buzzer_init();

    // PIN
    pin_code_init();

    // start the system in IDLE
    set_state(STATE_IDLE);

    uart_init(UART_BAUDRATE);
    app_console_init();
    uart_write_string("System ready\n");
    uart_write_string("Type: help\n");
}

// Main application loop - handles state machine
void app_run(void)
{
    app_console_handle(current_state == STATE_IDLE);

    switch (current_state)
    {
        case STATE_IDLE:
            app_rfid_check();
            update_blue_blink();
            // Only the green start button is active in this state.
            if (green_button_pressed())
            {
                set_state(STATE_INPUT_AWAIT);
            }
            break;

        case STATE_INPUT_AWAIT:
        {
            char key;

            // The start button is ignored here; only the keypad is used.
            update_red_blink();
            update_green_blink();

            if (input_timeout_expired())
            {
                set_state(STATE_IDLE);
            }
            else
            {
                key = keypad_scan();

                if (key != '\0')
                {
                    blink_green_once();
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
                set_state(STATE_IDLE);
            }
            break;

        default:
            set_state(STATE_IDLE);
            break;
    }
}
