/*
 * SHDLC-Generator: 0.8.2
 * Yaml Version: 0.1.0
 * Template Version: 0.7.0
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>  // printf

#include "sen44_uart.h"
#include "sensirion_common.h"
#include "sensirion_uart_hal.h"

/* TO USE CONSOLE OUTPUT (printf) YOU MAY NEED TO ADAPT THE
 * INCLUDE ABOVE OR DEFINE IT ACCORDING TO YOUR PLATFORM.
 * #define printf(...)
 */

int main(void) {
    int16_t error = 0;

    error = sensirion_uart_hal_init();
    if (error) {
        printf("Error initializing UART: %i\n", error);
        return error;
    }

    error = sen44_device_reset();
    if (error) {
        printf("Error executing sen44_device_reset(): %i\n", error);
    }

    unsigned char serial_number[255];
    uint8_t serial_number_size = 255;

    error = sen44_get_serial_number(serial_number, serial_number_size);

    if (error) {
        printf("Error executing sen44_get_serial_number(): %i\n", error);
    } else {
        printf("Serial number: %s\n", serial_number);
    }

    uint8_t firmware_major;
    uint8_t firmware_minor;
    bool firmware_debug;
    uint8_t hardware_major;
    uint8_t hardware_minor;
    uint8_t protocol_major;
    uint8_t protocol_minor;

    error = sen44_get_version(&firmware_major, &firmware_minor, &firmware_debug,
                              &hardware_major, &hardware_minor, &protocol_major,
                              &protocol_minor);

    if (error) {
        printf("Error executing sen44_get_version(): %i\n", error);
    } else {
        if (firmware_debug) {
            printf("Development firmware version: ");
        }
        printf("Firmware: %u.%u, Hardware: %u.%u, protocol: %u.%u\n",
               firmware_major, firmware_minor, hardware_major, hardware_minor,
               protocol_major, protocol_minor);
    }

    // Start Measurement

    error = sen44_start_measurement();

    if (error) {
        printf("Error executing sen44_start_measurement(): %i\n", error);
    }

    for (;;) {
        // Read Measurement
        sensirion_uart_hal_sleep_usec(1000000);

        uint16_t mass_concentration_pm1p0;
        uint16_t mass_concentration_pm2p5;
        uint16_t mass_concentration_pm4p0;
        uint16_t mass_concentration_pm10p0;
        int16_t voc_index;
        int16_t ambient_humidity;
        int16_t ambient_temperature;

        error = sen44_read_measured_mass_concentration_and_ambient_values(
            &mass_concentration_pm1p0, &mass_concentration_pm2p5,
            &mass_concentration_pm4p0, &mass_concentration_pm10p0, &voc_index,
            &ambient_humidity, &ambient_temperature);

        if (error) {
            printf("Error executing "
                   "sen44_read_measured_mass_concentration_and_ambient_values()"
                   ": %i\n",
                   error);
        } else {
            printf("Mass concentration pm1p0: %u\n", mass_concentration_pm1p0);
            printf("Mass concentration pm2p5: %u\n", mass_concentration_pm2p5);
            printf("Mass concentration pm4p0: %u\n", mass_concentration_pm4p0);
            printf("Mass concentration pm10p0: %u\n",
                   mass_concentration_pm10p0);
            printf("Voc index: %.1f\n", voc_index / 10.0f);
            printf("Ambient humidity: %.2f%%RH\n", ambient_humidity / 100.0f);
            printf("Ambient temperature: %.2f°C\n",
                   ambient_temperature / 200.0f);
        }
    }

    error = sen44_stop_measurement();

    if (error) {
        printf("Error executing sen44_stop_measurement(): %i\n", error);
    }

    return 0;
}
