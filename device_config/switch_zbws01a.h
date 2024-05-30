/********************************************************************************************************
 * @file    switch_zbws01a.h
 *
 * @brief   Board definition for tuya TS0041 - Lonsonho switch Model ZBWS01A
 *
 * @author  doctor64
 * @date    2024
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
//#include "app_cfg.h"

/* Debug mode config */
#define	UART_PRINTF_MODE				1
#define USB_PRINTF_MODE         		0
//#define BAUDRATE 						1000000//1M


//zigbee model id
#define ZCL_BASIC_MODEL_ID          {20,'F','i','x','e','d','_','Z','B','W','S','0','1','A','_','T','S','0','0','4','1'}

//
#define HARDWARE_REV                0x01

//OTA image type, define device type
#ifdef BUILD_BOOTLOADER
#define IMAGE_TYPE                          ((CHIP_TYPE << 8) | IMAGE_TYPE_BOOTLOADER) //0x03FF
#else
#define IMAGE_TYPE                          ((CHIP_TYPE << 8) | IMAGE_TYPE_SWITCH_ZBWS01A) //0x03B1
#endif

// BUTTONS definition
// Net Key button
#define BUTTON1                     GPIO_PB5
#define PB5_FUNC                    AS_GPIO
#define PB5_OUTPUT_ENABLE           0
#define PB5_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PB5         PM_PIN_PULLDOWN_100K//PM_PIN_UP_DOWN_FLOAT//PM_PIN_PULLUP_10K//PM_PIN_PULLDOWN_100K
#define PM_WAKEUP_LEVEL_BUTTON1     PM_WAKEUP_LEVEL_HIGH
#define HAVE_NET_BUTTON             1

//SW2 button
#define BUTTON2                     GPIO_PD2
#define PD2_FUNC                    AS_GPIO
#define PD2_OUTPUT_ENABLE           0
#define PD2_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PD2         PM_PIN_PULLUP_10K
#define PM_WAKEUP_LEVEL_BUTTON2     PM_WAKEUP_LEVEL_LOW
#define HAVE_1_BUTTON               1

#define TOTAL_BUTTONS_NUM           2

// LEDS definition
#define LED1                        GPIO_PD4
#define PD4_FUNC                    AS_GPIO
#define PD4_OUTPUT_ENABLE           1
#define PD4_INPUT_ENABLE            0
//to disable led blink on wake, led off with 1
#define PD4_DATA_OUT                1
//ZBWS01A TS0041 have LEDs active low
#define LED_ON						0//1
#define LED_OFF						1//0
#define HAVE_1_LED                  1
#define LED_POWER                   LED1


// UART
#if ZBHCI_UART
    #error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
    #define DEBUG_INFO_TX_PIN       GPIO_PB1//print
#endif

// Button codes starts from 0 and incremental
#define  VK_NET  0x00
#define  VK_SW1  0x01


#define KB_MAP_NORMAL   {\
        {VK_NET,}, \
        {VK_SW1,}, }


#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   { BUTTON1,  BUTTON2}
#define KB_SCAN_LEVEL  {       1,        0}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
