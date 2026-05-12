#include "app_rfid.h"

#include "mfrc522.h"
#include "millis.h"
#include "uart.h"

#define RFID_COOLDOWN_MS 1000UL

static millis_t last_rfid_read_time;

void app_rfid_init(void)
{
    char buffer[32];
    uint8_t version;

    last_rfid_read_time = 0;
    mfrc522_init();
    version = mfrc522_get_version();

    buffer[0] = "0123456789ABCDEF"[(version >> 4) & 0x0F];
    buffer[1] = "0123456789ABCDEF"[version & 0x0F];
    buffer[2] = '\0';

    uart_write_string("MFRC522 version: 0x");
    uart_write_string(buffer);
    uart_write_string("\n");
}

uint8_t app_rfid_check(void)
{
    uint8_t atqa[2];
    uint8_t atqa_len = 0;
    mfrc522_uid_t uid;
    mfrc522_status_t status;

    if ((millis() - last_rfid_read_time) < RFID_COOLDOWN_MS)
    {
        return 0;
    }

    status = mfrc522_request_a(atqa, &atqa_len);
    if (status != MFRC522_OK)
    {
        return 0;
    }

    status = mfrc522_anticoll_select(&uid);
    if (status != MFRC522_OK)
    {
        return 0;
    }

    uart_write_string("RFID UID: ");
    for (uint8_t i = 0; i < uid.size; i++)
    {
        uint8_t value = uid.uid[i];
        char buffer[3];
        buffer[0] = "0123456789ABCDEF"[(value >> 4) & 0x0F];
        buffer[1] = "0123456789ABCDEF"[value & 0x0F];
        buffer[2] = '\0';
        uart_write_string(buffer);
        if (i < uid.size - 1U)
        {
            uart_write_string(" ");
        }
    }
    uart_write_string("\n");

    last_rfid_read_time = millis();
    mfrc522_halt();
    return 1;
}
