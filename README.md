# Arduino Uno Shift Register Access Control

A homework project for Arduino Uno / ATmega328P. This system extends a PIN lock with a 74HC595 latch, RGB LEDs, RFID, and support for future RTC, buzzer, and servo drivers.

## What it does

- PIN access state machine: `IDLE`, `INPUT_AWAIT`, `ACCESS_GRANTED`
- 4x4 keypad scanning using a 74HC595 latch for row control
- RGB LED control through the 74HC595
- UART PIN change command
- EEPROM PIN storage
- SPI support and MFRC522 RFID initialization
- Simple RFID UID output via UART

## Hardware overview

- Arduino Uno / ATmega328P
- 74HC595 latch
- 4x4 keypad
- MFRC522 RFID reader
- RGB LED
- A0 start button with internal pull-up

## Latch Pins

- `DS` -> `D11 / PB3`
- `SHCP` -> `D13 / PB5`
- `STCP` -> `D2 / PD2`

Latch bit use:

- Rows 0–3: bits 0–3
- Red LED: bit 5
- Green LED: bit 6
- Blue LED: bit 7

## RFID SPI pins

- `SDA/SS` -> `PB2`
- `MOSI` -> `PB3`
- `MISO` -> `PB4`
- `SCK` -> `PB5`
- `RST` -> `PB0`

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
