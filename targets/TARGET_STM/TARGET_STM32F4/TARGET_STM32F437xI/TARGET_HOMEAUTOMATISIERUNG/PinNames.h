/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
    PA_0  = 0x00, PA_1  = 0x01, PA_2  = 0x02, PA_3  = 0x03,
    PA_4  = 0x04, PA_5  = 0x05, PA_6  = 0x06, PA_7  = 0x07,
    PA_8  = 0x08, PA_9  = 0x09, PA_10 = 0x0A, PA_11 = 0x0B,
    PA_12 = 0x0C, PA_13 = 0x0D, PA_14 = 0x0E, PA_15 = 0x0F,

    PB_0  = 0x10, PB_1  = 0x11, PB_2  = 0x12, PB_3  = 0x13,
    PB_4  = 0x14, PB_5  = 0x15, PB_6  = 0x16, PB_7  = 0x17,
    PB_8  = 0x18, PB_9  = 0x19, PB_10 = 0x1A, PB_11 = 0x1B,
    PB_12 = 0x1C, PB_13 = 0x1D, PB_14 = 0x1E, PB_15 = 0x1F,

    PC_0  = 0x20, PC_1  = 0x21, PC_2  = 0x22, PC_3  = 0x23,
    PC_4  = 0x24, PC_5  = 0x25, PC_6  = 0x26, PC_7  = 0x27,
    PC_8  = 0x28, PC_9  = 0x29, PC_10 = 0x2A, PC_11 = 0x2B,
    PC_12 = 0x2C, PC_13 = 0x2D, PC_14 = 0x2E, PC_15 = 0x2F,

    PD_0  = 0x30, PD_1  = 0x31, PD_2  = 0x32, PD_3  = 0x33,
    PD_4  = 0x34, PD_5  = 0x35, PD_6  = 0x36, PD_7  = 0x37,
    PD_8  = 0x38, PD_9  = 0x39, PD_10 = 0x3A, PD_11 = 0x3B,
    PD_12 = 0x3C, PD_13 = 0x3D, PD_14 = 0x3E, PD_15 = 0x3F,
    
    PE_0  = 0x40, PE_1  = 0x41, PE_2  = 0x42, PE_3  = 0x43,
    PE_4  = 0x44, PE_5  = 0x45, PE_6  = 0x46, PE_7  = 0x47,
    PE_8  = 0x48, PE_9  = 0x49, PE_10 = 0x4A, PE_11 = 0x4B,
    PE_12 = 0x4C, PE_13 = 0x4D, PE_14 = 0x4E, PE_15 = 0x4F,


    PH_0  = 0x70, PH_1  = 0x71,

    // ADC Channels
    ADCx_IN3        = PA_3,
    ADCx_IN4        = PA_4,
    ADCx_IN5        = PA_5,
    ADCx_IN6        = PA_6,
    ADCx_IN8        = PB_0,
    ADCx_IN9        = PB_1,
    ADCx_IN10       = PC_0,

    // ADC internal channels
    ADC_TEMP        = 0xF0,
    ADC_VREF        = 0xF1,
    ADC_VBAT        = 0xF2,

    // UART
    USART1_RX       = PB_7,
    USART1_TX       = PB_6,
    USART2_RX       = PD_6,
    USART2_TX       = PD_5,
    USART3_RX       = PB_12,
    USART3_TX       = PB_10,
    USART3_CTS      = PB_13,
    USART3_RTS      = PB_14,
    UART8_RX        = PE_0,
    UART8_TX        = PE_1,

    // I2C1
    I2C1_SDA        = PB_7,
    I2C1_SCL        = PB_6,
    I2C2_SDA        = PB_11,
    I2C2_SCL        = PB_10,
    I2C3_SDA        = PC_9,
    I2C3_SCL        = PA_8,

    // SPI
    SPI1_MOSI       = PB_5,
    SPI1_MISO       = PB_4,
    SPI1_SCK        = PB_3,
    SPI1_NSS        = PA_15,
    SPI1_CS1        = PD_5,
    SPI1_CS2        = PD_6,
    SPI2_MOSI       = PB_15,
    SPI2_MISO       = PB_14,
    SPI2_SCK        = PB_13,
    SPI2_NSS        = PB_12,
    SPI2_CS         = SPI2_NSS,
    SPI3_MOSI       = PC_12,
    SPI3_MISO       = PC_11,
    SPI3_SCK        = PC_10,
    SPI3_NSS        = PA_15,
    SPI3_CS         = SPI3_NSS,
    SPI4_MOSI       = PE_6,
    SPI4_MISO       = PE_5,
    SPI4_SCK        = PE_2,
    SPI4_NSS        = PE_4,
    SPI4_CS         = SPI4_NSS,

    // I2S
    I2S2_MCK        = PC_6,
    I2S2_SD         = PC_3,
    I2S2_ext_SD     = PC_2,
    I2S2_WS         = PB_9,
    I2S2_CK         = PD_3,
    
    // CAN
    CAN1_RX         = PD_0,
    CAN1_TX         = PD_1,
    CAN2_RX         = PB_12,
    CAN2_TX         = PB_13,

    // Timer
    TIM1_CH1N       = PE_8,
    TIM1_CH1        = PE_9,
    TIM1_CH2N       = PE_10,
    TIM1_CH2        = PE_11,
    TIM1_CH3N       = PE_12,
    TIM1_CH3        = PE_13,
    TIM1_CH4        = PE_14,
    TIM2_CH1        = PA_5,
    TIM2_CH4        = PA_3,
    TIM3_CH1        = PA_6,
    TIM3_CH2        = PA_7,
    TIM3_CH3        = PB_0,
    TIM3_CH4        = PB_1,
    TIM4_CH1        = PD_12,
    TIM4_CH2        = PD_13,
    TIM4_CH3        = PD_14,
    TIM4_CH4        = PD_15,

    /**** USB pins ****/
    USB_OTG_FS_DM   = PA_11,
    USB_OTG_FS_DP   = PA_12,
    USB_OTG_FS_ID   = PA_10,
    USB_OTG_FS_SOF  = PA_8,
    USB_OTG_FS_VBUS = PA_9,

    // AIN
    ADC_IN1         = ADCx_IN3,
    ADC_IN2         = ADCx_IN4,
    ADC_IN3         = ADCx_IN5,
    ADC_IN4         = ADCx_IN6,
    ADC_IN5         = ADCx_IN8,
    ADC_IN6         = ADCx_IN9,
    ADC_I_12V       = ADCx_IN10,

    // PWM
    PWM_OUT11       = TIM1_CH1N,
    PWM_OUT12       = TIM1_CH1,
    PWM_OUT13       = TIM1_CH2N,
    PWM_OUT14       = TIM1_CH2,
    PWM_OUT15       = TIM1_CH3N,
    PWM_OUT16       = TIM1_CH3,
    PWM_OUT17       = TIM1_CH4,
    PWM_OUT21       = TIM2_CH1,
    PWM_OUT22       = TIM2_CH4,
    PWM_OUT31       = TIM3_CH1,
    PWM_OUT32       = TIM3_CH2,
    PWM_OUT33       = TIM3_CH3,
    PWM_OUT34       = TIM3_CH4,
    PWM_OUT41       = TIM4_CH1,
    PWM_OUT42       = TIM4_CH2,
    PWM_OUT43       = TIM4_CH3,
    PWM_OUT44       = TIM4_CH4,

    // EXTI
    PB_EXTI0        = PB_0,
    PB_EXTI1        = PB_1,
    PA_EXTI2        = PA_2,
    PA_EXTI3        = PA_3,
    PA_EXTI4        = PA_4,
    PA_EXTI5        = PA_5,
    PA_EXTI6        = PA_6,
    PD_EXTI12       = PD_12,
    PD_EXTI13       = PD_13,
    PD_EXTI14       = PD_14,
    PD_EXTI15       = PD_15,
    PE_EXTI8        = PE_8,
    PE_EXTI9        = PE_9,
    PE_EXTI10       = PE_10,
    PE_EXTI11       = PE_11,
    PE_EXTI12       = PE_12,
    PE_EXTI13       = PE_13,
    PE_EXTI14       = PE_14,
    
    // Interface
    INT_USART_RX    = USART3_RX,
    INT_USART_TX    = USART3_TX,
    INT_USART_CTS   = USART3_CTS,
    INT_USART_RTS   = USART3_RTS,
    INT_CAN_RX      = CAN2_RX,
    INT_CAN_TX      = CAN2_TX,
    INT_I2C_SDA     = I2C2_SDA,
    INT_I2C_SCL     = I2C2_SCL,
    INT_SPI_MOSI    = SPI2_MOSI,
    INT_SPI_MISO    = SPI2_MISO,
    INT_SPI_SCK     = SPI2_SCK,
    INT_SPI_CS      = SPI2_CS,

    /**** ETHERNET pins ****/
    ETH_REF_CLK     = PA_1,
    ETH_MDIO        = PA_2,
    ETH_CRS_DV      = PA_7,
    ETH_RX_ER       = PB_10,
    ETH_TX_EN       = PB_11,
    ETH_TXD0        = PB_12,
    ETH_TXD1        = PB_13,
    ETH_MDC         = PC_1,
    ETH_RXD0        = PC_4,
    ETH_RXD1        = PC_5,

    // Flash
    FLASH_SPI_MOSI  = SPI4_MOSI,
    FLASH_SPI_MISO  = SPI4_MISO,
    FLASH_SPI_SCK   = SPI4_SCK,
    FLASH_SPI_CS    = SPI4_CS,
    FLASH_SPI_WP    = PE_3,
    FLASH_SPI_HOLD  = PC_13,

    // Power
    PoE_I_12V       = ADC_I_12V,
    PoE_IO1         = PC_14,
    PoE_12V_EN      = PE_7,

    // STDIO for console print
    STDIO_UART_TX   = UART8_TX,
    STDIO_UART_RX   = UART8_RX,

    /**** OSCILLATOR pins ****/
    RCC_OSC_IN      = PH_0,
    RCC_OSC_OUT     = PH_1,
    RCC_MCO_1       = PA_8,
    RCC_MCO_2       = PC_9,

    /**** DEBUG pins ****/
    SYS_JTCK_SWCLK  = PA_14,
    SYS_JTMS_SWDIO  = PA_13,

    // ST-Link
    USBRX           = STDIO_UART_RX,
    USBTX           = STDIO_UART_TX,
    SWDIO           = SYS_JTCK_SWCLK, 
    SWCLK           = SYS_JTMS_SWDIO,

    // Not connected
    LED1            = PA_0,
    NC              = (int)0xFFFFFFFF
} PinName;

#define ACTIVE_HIGH_POLARITY    1
#define ACTIVE_LOW_POLARITY     0

#ifdef __cplusplus
}
#endif

#endif
