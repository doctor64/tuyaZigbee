/********************************************************************************************************
 * @file    board_826x_dongle.h
 *
 * @brief   This is the header file for board_826x_dongle
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

//KEY
#define	BUTTON1 					GPIO_PD2
#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			0
#define PD2_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD2			PM_PIN_PULLUP_10K

#define	BUTTON2 					GPIO_PC5
#define PC5_FUNC					AS_GPIO
#define PC5_OUTPUT_ENABLE			0
#define PC5_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PC5			PM_PIN_PULLUP_10K

//LED
/***************************************************************
* LED_R		GPIO_PC2	//D3 -- red			PWM2
* LED_G		GPIO_PC3	//D1 -- green		PWM3
* LED_B		GPIO_PB6	//D2 -- blue		PWM5
* LED_W		GPIO_PB4	//D4 -- white		PWM4
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)

#define PC2_FUNC					AS_PWM
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0

#define PC3_FUNC					AS_PWM
#define PC3_OUTPUT_ENABLE			1
#define PC3_INPUT_ENABLE			0

#define PB6_FUNC					AS_PWM
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define PWM_R_CHANNEL_SET()			PWM2_CFG_GPIO_C2()
#define PWM_R_CHANNEL				2//PWM2

#define PWM_G_CHANNEL_SET()			PWM3_CFG_GPIO_C3()
#define PWM_G_CHANNEL				3//PWM3

#define PWM_B_CHANNEL_SET()			PWM5_CFG_GPIO_B6()
#define PWM_B_CHANNEL				5//PWM5

#define R_LIGHT_PWM_CHANNEL			PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL			PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL			PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()			PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()			PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()			PWM_B_CHANNEL_SET()

#define LED_W						GPIO_PB4

#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define LED_POWER					NULL
#define LED_PERMIT					LED_W

#else

//PWM configuration, LED_B as warm light, LED_W as cool light.
#define PB6_FUNC					AS_PWM
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define PB4_FUNC					AS_PWM
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define PWM_B_CHANNEL_SET()			PWM5_CFG_GPIO_B6()
#define PWM_B_CHANNEL				5//PWM5

#define PWM_W_CHANNEL_SET()			PWM4_CFG_GPIO_B4()
#define PWM_W_CHANNEL				4//PWM4

#define WARM_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

#define LED_R						GPIO_PC2
#define LED_G						GPIO_PC3

#define PC2_FUNC					AS_GPIO
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0

#define PC3_FUNC					AS_GPIO
#define PC3_OUTPUT_ENABLE			1
#define PC3_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#endif

//UART
#if	ZBHCI_UART
	PC2 & PC3 are configured as LED pins
#endif

//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PB5//print
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
