/********************************************************************************************************
 * @file    board_ZG-102ZL.h
 *
 * @brief   This is the header file for board_ZG-102ZL contact sensor with light
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

// BUTTON
// Net Key button
#define	BUTTON1 					GPIO_PD2
#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			0
#define PD2_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD2			PM_PIN_PULLUP_10K

// contact sensor
#define	BUTTON2 					GPIO_PB4
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			0
#define PB4_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PB4			PM_PIN_PULLUP_10K

//LED
#define LED1						GPIO_PB5
#define PB5_FUNC					AS_GPIO
#define PB5_OUTPUT_ENABLE			1
#define PB5_INPUT_ENABLE			0
//to disable led blink on wake, led off with 1
#define PB5_DATA_OUT                1

//have LEDs active low
#define LED_ON						0//1
#define LED_OFF						1//0

#define	PM_WAKEUP_LEVEL		  		PM_WAKEUP_LEVEL_LOW

#define LIGHT_SENSOR_DRIVER			GPIO_PC2
#define PC2_FUNC					AS_GPIO
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0
#define PC2_DATA_OUT                1

#define HAVE_LIGHT_SENSOR           1
#define LIGHT_SENSOR_SENSE			GPIO_PC4
#define PC4_FUNC					AS_GPIO
#define PC4_OUTPUT_ENABLE			0
#define PC4_INPUT_ENABLE			0
#define PC4_DATA_OUT                0

// Send OnOf commands on sensor actions
#define HAVE_ONOFF_SEND				1

// #define PC3_FUNC					AS_GPIO
// #define PC3_OUTPUT_ENABLE			1
// #define PC3_INPUT_ENABLE			0
// #define PC3_DATA_OUT                1
// #define PC1_FUNC					AS_GPIO
// #define PC1_OUTPUT_ENABLE			1
// #define PC1_INPUT_ENABLE			0
// #define PC1_DATA_OUT                1


// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PB1 //print
#endif

enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1,}, \
		{VK_SW2,}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1,  BUTTON2}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
