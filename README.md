# Boba-Fett-Moff-Gideon-LED-Chest-Display

Boba Fett / Moff Gideon LED Chest Display for cosplay or props.

## Features
- Multiplexed LED bar display for chest armor effects
- Gradual power-up sequence to detect brownout correctly
- Battery voltage monitoring with low-voltage deep sleep and green LED alert

## Usage
- Power the display with a suitable battery (5–10 V recommended, 5-18 V suitable)
- LEDs gradually light up on startup
- If battery voltage is too low, the circuit enters deep sleep and indicates the error with the green LED
- It is possible to connect the powerbank directly to the pins used for the 9 V battery, but the consumption of the circuit is approximately 100 mA, so it is important to keep the powerbank on. (the data D+ and D- are connected by using the 20 k resisotr)

## Programming
- Microcontroller: **ATmega328PB**
- Programming via **USB-ASP** device
- Use [AVRDUDE](https://github.com/avrdudes/avrdude) for direct programming; select "USBasp (USBasp ISP and TPI programmer)"
- Fuse settings for this project:
  - Low: `0xE2`
  - High: `0xD9`
  - Extended: `0xFD`
  - Lock Bits: `0xFF`
  - Verify fuses with [Engbedded Fuse Calculator](https://www.engbedded.com/fusecalc/)

- Firmware source code is included in [SW](https://github.com/vasek9876/Boba-Fett-Moff-Gideon-LED-Chest-Display/tree/all/sw)

## Notes
- Ensure proper power supply to avoid brownout during startup (>5 V)
- Multiplexing is handled in firmware
- The rapid switching of LEDs may generate audible noise from the power supply or nearby components; this is normal and does not indicate a fault.

