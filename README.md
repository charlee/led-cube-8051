8x8x8 LED Cube with 8051
=========================

This repository is the source code for my LED Cube projects driven with an 8051 (STC12C5A60S2) MCU.


Watch the LED cube animation on YouTube:

[![LED Cube](https://img.youtube.com/vi/Z23BWwKdips/0.jpg)](https://youtu.be/Z23BWwKdips)

You can find the schematic and PCB [here](schematics/README.md).

Also check the [gallery page](gallery/README.md) for some pictures.


## About the MCU

The LED Cube requires row scanning in order to control each individual LED.
Initially I tried to use an AT89C52 with a 12MHz crystal, but later I realized that 
AT89C52 is too slow to produce a stable row scanning.

After some experiments, I chose an [STC12C5A60S2](http://www.stcmicro.com/datasheet/STC12C5A60S2-en.pdf) with a 25MHz crystal.
Compared to AT89C52 which requires 12 cycles per instruction, STC12C5A60S2 only requires 1 cycle per instruction, 
which means it is 12x faster than AT89C52. With an 25Hz crystal it is approximately 24x faster.

I used Timer 0 interrupt to handle the row scanning. With STC12C5A60S2, the timer counter can be set to 5000
(so that the rowscan happens every 5000 / 25MHz = 0.2ms, and the whole cube is refreshed every 0.2ms * 8 = 1.6ms).
This will produce a relatively stable display to human eyes.

> A larger counter greater than 5000 causes the LEDs to flicker, and a smaller counter could not provide enough time for the rowscan code to be executed.


## Development

This project is originally developed under Windows.

Install [SDCC](http://sdcc.sourceforge.net/)

Install `make`:

    > chocolatey.exe install make

To generate the HEX file, simply run `make` under the `src` directory.