# SMC_JXC_Controller

This repository provides a simple interface for controlling SMC devices such as LECP6, LECA6, JXC51, and JXC61 using Parallel I/O. 

IN3-5, OUT3-5, AREA, and ESTOP are not utilized so that the board Im using can handle two actuators at the same time.

Additionally, the project includes support for an I/O expansion board based on I2C. This feature can be enabled or disabled by commenting or uncommenting the `#define __USE_IO_EXPAND__` directive in `SMC_JXC51.h`.

## Environment

I do my work on a PIC16 microcontroller using MPLAB IDE. I believe he design should be easily adapted to other platforms.

## How to use

To use this interface, call the `taskJXC51()` function periodically, with a recommended interval of no less than 50ms between each call. Adjust the I/O settings to match specific board configuration. Then use other functions in `SMC_JXC51.h`.