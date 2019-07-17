/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
//    PB_9  = 0x19,
//    PB_10 = 0x1A,
//    PB_11 = 0x1B,
//    PB_12 = 0x1C,
//    PB_13 = 0x1D,
//    PB_14 = 0x1E,
//    PB_15 = 0x1F,

//    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

//    PH_0  = 0x70,
//    PH_1  = 0x71,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VLCD = 0xF2,

    // Arduino connector namings
    // Note: The Arduino connector is not present on this board.
    // We keep these definitions for compatibility with Nucleo code examples.
    A0          = PA_1,
    A1          = PA_2,
    A2          = PA_3,
    A3          = PA_4,
    A4          = PB_0,
    A5          = PB_1,
    D0          = PA_10,
    D1          = PA_9,
    D2          = PA_8,
    D3          = PA_6,
    D4          = PA_13,
    D5          = PA_7,
    D6          = PB_11,
    D7          = PC_13,
    D8          = PC_14,
    D9          = PA_15,
    D10         = PA_5,
    D11         = PB_15,
    D12         = PB_14,
    D13         = PB_13,
    D14         = PB_9,
    D15         = PB_8,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX = PA_9,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX = PA_10,
#endif

    // Generic signals namings
//    LED1        = PB_4,
//    LED2        = PA_5,
//    LED3        = PB_4,
//    LED4        = PA_5,
    USER_BUTTON = PA_0,
    // Standardized button names
    BUTTON1 = USER_BUTTON,
    SERIAL_TX   = STDIO_UART_TX,
    SERIAL_RX   = STDIO_UART_RX,
    USBTX       = STDIO_UART_TX,
    USBRX       = STDIO_UART_RX,
    I2C_SCL     = PB_6,
    I2C_SDA     = PB_7,
    SPI_MOSI    = PB_5,
    SPI_MISO    = PB_4,
    SPI_SCK     = PB_3,
    SPI_CS      = PB_1,
    SPI_CS1     = SPI_CS,
    SPI_CS2     = PB_2,
    SPI_IRQ     = PA_9,
    ACC_ST      = PA_4,
    ACC_EN      = PA_5,
    BT_RESET    = PA_7,
    ADC1        = PA_1,
    ADC2        = PA_2,
    ADC3        = PA_3,

    /**** USB pins ****/
    USB_DM = PA_11,
    USB_DP = PA_12,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,

    /**** DEBUG pins ****/
    SYS_SWCLK = PA_14,
    SYS_SWDIO = PA_13,
    SYS_WKUP1 = PA_0,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
