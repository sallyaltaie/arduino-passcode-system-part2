CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRSIZE=avr-size
OBJISP=avrdude
MCU=atmega328p
CFLAGS=-Wall -Wextra -Wundef -pedantic \
	-Os -g -DF_CPU=16000000UL -mmcu=$(MCU) -DBAUD=19200 \
	-Iinclude
LDFLAGS=-mmcu=$(MCU)
PORT=/dev/tty.usbmodem11201

BIN=avrdemo
SOURCES=main.c \
	src/app.c \
	src/app_console.c \
	src/app_rfid.c \
	src/command.c \
	src/led.c \
	src/pin_code.c \
	src/uart.c \
	src/ring_buffer.c \
	src/millis.c \
	src/keypad.c \
	src/mfrc522.c \
	src/i2c.c \
	src/rtc.c \
	src/buzzer.c \
	src/spi.c \
	src/shift_register.c \
	src/servo.c

OUTPUTDIR=build
OBJS=$(addprefix $(OUTPUTDIR)/,$(SOURCES:.c=.o))
ELF=$(OUTPUTDIR)/$(BIN).elf
HEX=$(OUTPUTDIR)/$(BIN).hex
MAP=$(OUTPUTDIR)/$(BIN).map

all: $(HEX)

$(OUTPUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MD -o $@ -c $<

$(ELF): $(OBJS)
	$(CC) -Wl,-Map=$(MAP) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S -s $< > $@

isp: $(HEX)
	$(OBJISP) -F -V -c arduino -p $(MCU) -P $(PORT) -U flash:w:$<

clean:
	rm -f $(ELF) $(HEX) $(MAP) $(OBJS) $(OBJS:.o=.d)
	rm -rf $(OUTPUTDIR)

-include $(OBJS:.o=.d)

.PHONY: all clean isp
