/********************************************************************************************************
 * @file    board_b91_evk.h
 *
 * @brief   This is the header file for board_b91_evk
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
#define BUTTON1               		GPIO_PC2
#define PC2_FUNC			  		AS_GPIO
#define PC2_OUTPUT_ENABLE	  		0
#define PC2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC2	  		GPIO_PIN_PULLDOWN_100K

#define BUTTON2               		GPIO_PC0
#define PC0_FUNC			  		AS_GPIO
#define PC0_OUTPUT_ENABLE	  		0
#define PC0_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC0	  		GPIO_PIN_PULLDOWN_100K

#define BUTTON3               		GPIO_PC3
#define PC3_FUNC			  		AS_GPIO
#define PC3_OUTPUT_ENABLE	  		0
#define PC3_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC3	  		GPIO_PIN_PULLUP_10K

#define BUTTON4               		GPIO_PC1
#define PC1_FUNC			  		AS_GPIO
#define PC1_OUTPUT_ENABLE	  		0
#define PC1_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC1	  		GPIO_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R			GPIO_PB7	//D4 -- red
* LED_G			GPIO_PB5	//D2 -- green
* LED_B			GPIO_PB4	//D1 -- blue
* LED_W			GPIO_PB6	//D3 -- white
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)
	#error "To do!"
#else
//PWM configuration, LED_R as warm light, LED_B as cool light.
#define LED_R						GPIO_PB7
#define LED_B						GPIO_PB4

#define PWM_R_CHANNEL				2	//PWM2
#define PWM_R_CHANNEL_SET()			do{\
										pwm_set_pin(LED_R); \
									}while(0)

#define PWM_B_CHANNEL				0	//PWM0
#define PWM_B_CHANNEL_SET()			do{\
										pwm_set_pin(LED_B); \
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_R_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_R_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()

//LED_W and LED_G as GPIO.
#define LED_W						GPIO_PB6
#define LED_G						GPIO_PB5

#define PB6_FUNC					AS_GPIO
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define PB5_FUNC					AS_GPIO
#define PB5_OUTPUT_ENABLE			1
#define PB5_INPUT_ENABLE			0

#define LED_POWER					LED_W
#define LED_PERMIT					LED_G
#endif

// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
#endif


enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02,
	VK_SW3 = 0x03,
	VK_SW4 = 0x04
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1, VK_SW3}, \
		{VK_SW2, VK_SW4}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  	{GPIO_PC2, GPIO_PC0}
#define KB_SCAN_PINS   	{GPIO_PC3, GPIO_PC1}

#define	KB_LINE_MODE		0
#define	KB_LINE_HIGH_VALID	0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
