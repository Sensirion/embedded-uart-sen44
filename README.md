# Sensirion Embedded UART SEN44 Driver

This is a generic embedded driver for the Sensirion SEN44 sensor module. It enables
developers to communicate with the SEN44 sensor module on different hardware platforms
by only adapting the UART communication related source files.

<center><img src="images/SEN4x.png" width="500px"></center>

# Getting started

## Connecting the Sensor

Your sensor has the four different connectors: VCC, GND, SDA, SCL, SEL (the sixth connector will not be used for now).

<center><img src="images/SEN44_pinout.png" width="300px"></center>

| *Pin* | *Name* | *Description* | *Comments* |
|-------|--------|---------------|------------|
| 1     | VDD    | Supply Voltage | 5V ±10%
| 2     | GND    | Ground |
| 3     | RX     | UART: Receiving pin for communication | TTL 5V and LVTTL 3.3V compatible
|       | SDA    | I2C: Serial data input / output | TTL 5V and LVTTL 3.3V compatible
| 4     | TX     | UART: Transmission pin for communication | TTL 5V and LVTTL 3.3V compatible
|       | SCL    | I2C: Serial clock input | TTL 5V and LVTTL 3.3V compatible
| 5     | SEL    | Interface select | Leave floating or pull to VDD to select UART
|       |        |  | Pull to GND to select I2C
| 6     | NC     | Do not connect |

## Implement the UART Interface

So we need to adjust two files according to your platform.

### Edit `sensirion_uart_hal.c`

This file contains the implementation of the sensor communication, which
depends on your hardware platform. We provide function stubs for your
hardware's own implementation.
Sample implementations are available for some platforms:
[`sample-implementations`](sample-implementations). For Linux based platforms
like Raspberry Pi you can just replace the unimplemented HAL template with the
implementation in `sample-implementations/linux_user_space/`:

```
cp sample-implementations/linux_user_space/sensirion_uart_hal.c ./
```

### Edit `sensirion_config.h`

Skip this part for Linux based platforms since everything is already setup for
this case.

Otherwise you need to check if the libraries `<stdint.h>` and `<stdlib.h>` are
provided by your toolchain, compiler or system. If you have no idea on how to
do that you can skip this step for now and come back when you get errors
related to these names when compiling the driver.
The features we use from those libraries are type definitions for integer sizes
from `<stdint.h>` and `NULL` from `<stdlib.h>`. If they are not available you
need to specify the following integer types yourself:

* `int64_t` = signed 64bit integer
* `uint64_t` = unsigned 64bit integer
* `int32_t` = signed 32bit integer
* `uint32_t` = unsigned 32bit integer
* `int16_t` = signed 16bit integer
* `uint16_t` = unsigned 16bit integer
* `int8_t` = signed 8bit integer
* `uint8_t` = unsigned 8bit integer

In addition to that you will need to specify `NULL`. For both we have a
detailed template where you just need to fill in your system specific values.

Now we are ready to compile and run the example usage for your sensor.

## Compile and Run

Pass the source `.c` and header `.h` files in this folder into your C compiler
and run the resulting binary. This step may vary, depending on your platform.
Here we demonstrate the procedure for Linux based platforms:

1. Open up a terminal.
2. Navigate to the directory where this README is located.
3. Run `make` (this compiles the example code into one executable binary).
4. Run the compiled executable with `./sen44_uart_example_usage`
5. Now you should see the first measurement values appear in your terminal. As
   a next step you can adjust the example usage file or write your own main
   function to use the sensor.

# Background

## Files

### sensirion\_shdlc.[ch]

In these files you can find the implementation of the SHDLC protocol used by
Sensirion sensors. The functions in these files are used by the embedded driver
to build the correct frame out of data to be sent to the sensor or receive a
frame of data from the sensor and convert it back to data readable by your
machine. The functions in here calculate and check the checksum, reorder bytes
for different byte orders and build the correct formatted frame for your
sensor.

### sensirion\_uart\_hal.[ch]

These files contain the implementation of the hardware abstraction layer used
by Sensirion's UART embedded drivers. This part of the code is specific to the
underlying hardware platform. This is an unimplemented template for the user to
implement. In the `sample-implementations/` folder we provide implementations
for the most common platforms.

### sensirion\_config.h

In this file we keep all the included libraries for our drivers and global
defines. Next to `sensirion_uart_hal.c` *it's the only file you should need to
edit to get your driver working.*

### sensirion\_common.[ch]

In these files you can find some helper functions used by Sensirion's embedded
drivers. It mostly contains byte order conversions for different variable
types. These functions are also used by the I2C embedded drivers therefore they
are kept in their own file.
