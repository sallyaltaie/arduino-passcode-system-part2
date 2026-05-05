# Arduino Uno Shift Register Access Control

A homework project for Arduino Uno / ATmega328P. This system extends a PIN lock with a 74HC595 shift register, RGB LEDs, RFID, and support for future RTC, buzzer, and servo drivers.

## What it does

- PIN access state machine: `IDLE`, `INPUT_AWAIT`, `ACCESS_GRANTED`
- 4x4 keypad scanning using a shift register for row control
- RGB LED control through the 74HC595
- UART PIN change command
- EEPROM PIN storage
- SPI support and MFRC522 RFID initialization
- Simple RFID UID output via UART

## Hardware overview

- Arduino Uno / ATmega328P
- 74HC595 shift register
- 4x4 keypad
- MFRC522 RFID reader
- RGB LED
- PB0 start button with internal pull-up

## Shift register pins

- `DS` -> `PD0`
- `SHCP` -> `PD1`
- `STCP` -> `PB5`

Shift register bit use:

- Rows 0–3: bits 0–3
- Red LED: bit 4
- Green LED: bit 5
- Blue LED: bit 6

## RFID SPI pins

- `SDA/SS` -> `PB2`
- `MOSI` -> `PB3`
- `MISO` -> `PB4`
- `SCK` -> `PB5`
- `RST` -> `PB1`

## Build

```sh
make
```

Clean:

```sh
make clean
```

Flash (optional):

```sh
make isp
```

## Wokwi

Use `diagram.json` and `wokwi.toml`, then open the serial monitor at `9600` baud.

## UART PIN command

Use this command in `IDLE` mode:

```text
NEW PIN;<old pin>;<new pin>
```

Example:

```text
NEW PIN;1772;1234
```

## Structure

- `include/` headers for each module
- `src/` application and driver code
- `Makefile`, `diagram.json`, `wokwi.toml`

## Author
Sally Altaie

