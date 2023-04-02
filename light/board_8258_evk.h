/********************************************************************************************************
 * @file    board_8258_evk.h
 *
 * @brief   This is the header file for board_8258_evk
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
#define BUTTON1               GPIO_PD1
#define PD1_FUNC			  AS_GPIO
#define PD1_OUTPUT_ENABLE	  0
#define PD1_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD1	  PM_PIN_PULLUP_10K

#define BUTTON2               GPIO_PD2
#define PD2_FUNC			  AS_GPIO
#define PD2_OUTPUT_ENABLE	  0
#define PD2_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD2	  PM_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R			GPIO_PD0	//D5 -- red
* LED_G			GPIO_PD4	//D3 -- green
* LED_B			GPIO_PD5	//D2 -- blue
* LED_W			GPIO_PD3	//D4 -- yellow
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)
	#error "GPIO_PD0 can't be configured as PWM!"
#else

//PWM configuration, LED_B as warm light, LED_W as cool light.
#define LED_B						GPIO_PD5	//D2 -- blue		PWM0
#define LED_W						GPIO_PD3	//D4 -- yellow		PWM1_N

#define PWM_W_CHANNEL				1	//PWM1_N
#define PWM_W_CHANNEL_SET()			do{ \
										gpio_set_func(LED_W, AS_PWM1_N); \
										drv_pwm_n_invert(PWM_W_CHANNEL); \
									}while(0)

#define PWM_B_CHANNEL				0	//PWM0
#define PWM_B_CHANNEL_SET()			do{ \
										gpio_set_func(LED_B, AS_PWM0); \
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PD0
#define LED_G						GPIO_PD4

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define PD0_FUNC					AS_GPIO
#define PD0_OUTPUT_ENABLE			1
#define PD0_INPUT_ENABLE			0

#define LED_POWER					LED_R
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
