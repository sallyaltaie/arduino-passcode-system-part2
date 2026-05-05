# PIN Access System

An AVR-based PIN access system for Arduino Uno / ATmega328P.

The system starts in locked mode with the red LED on. Pressing the green start button enables PIN input from a 4x4 keypad. The default PIN is `1772`. A correct PIN grants access for 3 seconds, while a wrong PIN or timeout returns the system to locked mode.

## Features

- State machine with `IDLE`, `INPUT_AWAIT`, and `ACCESS_GRANTED`.
- Red LED is on in idle mode.
- Red LED blinks while waiting for PIN input.
- Green LED flashes shortly for each registered keypad press.
- 5 second timeout for PIN entry.
- 3 second access granted state.
- UART command for changing the PIN in idle mode.
- PIN is stored in EEPROM and survives reset.
- Wokwi simulation support.

## Hardware

Target:

- Arduino Uno
- ATmega328P
- 4x4 membrane keypad
- Red LED
- Green LED
- Green push button

Pin mapping:

| Component | Arduino Pin | AVR Pin |
| --- | --- | --- |
| Green LED | D2 | PD2 |
| Red LED | D3 | PD3 |
| Green button | D12 | PB4 |
| Keypad R1 | D11 | PB3 |
| Keypad R2 | D10 | PB2 |
| Keypad R3 | D9 | PB1 |
| Keypad R4 | D8 | PB0 |
| Keypad C1 | D7 | PD7 |
| Keypad C2 | D6 | PD6 |
| Keypad C3 | D5 | PD5 |
| Keypad C4 | D4 | PD4 |

## Requirements

The project uses `avr-gcc`, `avr-libc`, `make`, `avr-objcopy`, `avr-size`, and optionally `avrdude` for flashing.

## Setup On macOS

Install the AVR toolchain with Homebrew:

```sh
brew tap osx-cross/avr
brew install avr-gcc avrdude make
```

Build the firmware:

```sh
make
```

Flash to Arduino Uno, if the serial port in the `Makefile` is correct:

```sh
make isp
```

If needed, update the `PORT` variable in `Makefile`.

## Setup On Linux

Install dependencies on Debian/Ubuntu:

```sh
sudo apt update
sudo apt install gcc-avr avr-libc binutils-avr avrdude make
```

Build the firmware:

```sh
make
```

Flash to Arduino Uno:

```sh
make isp
```

If flashing fails, check the board port, usually something like `/dev/ttyACM0` or `/dev/ttyUSB0`, and update `PORT` in `Makefile`.

## Setup On Windows

Recommended options:

- Use Wokwi in the browser for simulation.
- Use WSL for building with the Linux instructions.
- Use Microchip/AVR tools or WinAVR-style toolchains if building directly on Windows.

Using WSL:

1. Install WSL with Ubuntu.
2. Open Ubuntu terminal.
3. Install dependencies:

```sh
sudo apt update
sudo apt install gcc-avr avr-libc binutils-avr avrdude make
```

4. Build:

```sh
make
```

For flashing from Windows, Wokwi is usually simpler. If flashing a physical Arduino, make sure the serial port is available to your toolchain and update `PORT` in `Makefile`.

## Build

From the project root:

```sh
make
```

Generated files:

```text
build/avrdemo.elf
build/avrdemo.hex
build/avrdemo.map
```

Clean build files:

```sh
make clean
```

## Run In Wokwi

This repository includes:

- `diagram.json`
- `wokwi.toml`

Steps:

1. Build the project with `make`.
2. Open the project in Wokwi.
3. Start the simulation.
4. Open the Serial Monitor at `9600` baud.
5. Press the green start button.
6. Enter `1 7 7 2` on the keypad.
7. The green LED should turn on for 3 seconds.
8. The system should return to idle mode.

## UART Command

The PIN can only be changed while the system is in `IDLE`.

Command format:

```text
NEW PIN;<old pin>;<new pin>
```

Example:
NEW PIN;1772;1234

After this command, the new PIN is `1234`. The value is stored in EEPROM.

## Project Structure

```text
.
├── main.c
├── include/
│   ├── app.h
│   ├── config.h
│   ├── keypad.h
│   ├── led.h
│   ├── millis.h
│   ├── pin_code.h
│   ├── pins.h
│   ├── ring_buffer.h
│   └── uart.h
├── src/
│   ├── app.c
│   ├── keypad.c
│   ├── led.c
│   ├── millis.c
│   ├── pin_code.c
│   ├── ring_buffer.c
│   └── uart.c
├── Makefile
├── diagram.json
└── wokwi.toml
```

## Main Modules

- `main.c` starts the application and runs the main loop.
- `src/app.c` contains the state machine and system behavior.
- `src/keypad.c` scans the 4x4 keypad.
- `src/led.c` controls the red and green LEDs.
- `src/millis.c` provides millisecond timing with Timer1.
- `src/uart.c` handles UART communication.
- `src/ring_buffer.c` buffers received UART characters.
- `src/pin_code.c` validates and stores the PIN in EEPROM.

## Author
Sally Altaie