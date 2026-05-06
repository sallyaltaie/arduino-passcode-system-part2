#include "rtc.h"
#include "i2c.h"

#define DS1307_ADDRESS 0x68
#define DS1307_SECONDS_REG 0x00
#define DS1307_REGISTER_COUNT 7

static uint8_t bcd_to_decimal(uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

static uint8_t ds1307_read_registers(uint8_t start_reg, uint8_t *buffer, uint8_t length)
{
    if (twi_start() != TWI_OK)
        return TWI_ERROR;

    if (twi_write((DS1307_ADDRESS << 1) | 0) != TWI_OK)
        return TWI_ERROR;

    if (twi_write(start_reg) != TWI_OK)
        return TWI_ERROR;

    if (twi_start() != TWI_OK)
        return TWI_ERROR;

    if (twi_write((DS1307_ADDRESS << 1) | 1) != TWI_OK)
        return TWI_ERROR;

    for (uint8_t i = 0; i < length; i++)
    {
        if (i < (length - 1))
        {
            buffer[i] = twi_read_ack();
        }
        else
        {
            buffer[i] = twi_read_nack();
        }
    }

    twi_stop();
    return TWI_OK;
}

void rtc_init(void)
{
    twi_init(100000UL);
}

uint8_t rtc_read_time(rtc_time_t *time)
{
    uint8_t data[DS1307_REGISTER_COUNT];

    if (ds1307_read_registers(DS1307_SECONDS_REG, data, DS1307_REGISTER_COUNT) != TWI_OK)
    {
        return TWI_ERROR;
    }

    time->seconds = bcd_to_decimal(data[0] & 0x7F);
    time->minutes = bcd_to_decimal(data[1] & 0x7F);

    if (data[2] & 0x40)
    {
        uint8_t hour_bcd = data[2] & 0x1F;
        time->hours = bcd_to_decimal(hour_bcd);

        if (data[2] & 0x20)
        {
            if (time->hours < 12)
            {
                time->hours += 12;
            }
        }
        else if (time->hours == 12)
        {
            time->hours = 0;
        }
    }
    else
    {
        time->hours = bcd_to_decimal(data[2] & 0x3F);
    }

    time->day = data[3] & 0x07;
    time->date = bcd_to_decimal(data[4] & 0x3F);
    time->month = bcd_to_decimal(data[5] & 0x1F);
    time->year = bcd_to_decimal(data[6]);

    return TWI_OK;
}
