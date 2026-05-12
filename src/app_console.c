#include "app_console.h"
#include "command.h"
#include "uart.h"

#define CMD_BUFFER_SIZE 32

static char cmd_buffer[CMD_BUFFER_SIZE];
static unsigned char cmd_index;

static void app_console_process_command(uint8_t commands_allowed)
{
    if (cmd_index == 0)
    {
        return;
    }

    cmd_buffer[cmd_index] = '\0';

    if (commands_allowed)
    {
        command_process(cmd_buffer);
    }
    else
    {
        uart_write_string("PIN can only be changed in IDLE\n");
    }

    cmd_index = 0;
}

void app_console_init(void)
{
    cmd_index = 0;
}

void app_console_handle(uint8_t commands_allowed)
{
    char c;

    while (uart_read_char(&c))
    {
        uart_write_char(c);

        if (c == '\r' || c == '\n')
        {
            uart_write_string("\n");
            app_console_process_command(commands_allowed);
        }
        else if (cmd_index < (CMD_BUFFER_SIZE - 1))
        {
            cmd_buffer[cmd_index++] = c;
        }
        else
        {
            cmd_index = 0;
            uart_write_string("\nCommand too long\n");
        }
    }
}
