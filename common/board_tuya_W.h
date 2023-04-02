/********************************************************************************************************
 * @file    board_tuya_W.h
 * TyYa TS0501B _TZ3210_9q49basr
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
#define BUTTON1               GPIO_PC0
#define PC0_FUNC			  AS_GPIO
#define PC0_OUTPUT_ENABLE	  0
#define PC0_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PC0	  PM_PIN_PULLUP_10K

#define BUTTON2               GPIO_PD4
#define PD4_FUNC			  AS_GPIO
#define PD4_OUTPUT_ENABLE	  0
#define PD4_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD4	  PM_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R			GPIO_PB4	//B4 -- red             PWM4 mux 3
* LED_G			GPIO_PC3	//C3 -- green           PWM1 mux 4
* LED_B			GPIO_PD2	//D2 -- blue            PWM3 mux 2
* LED_CW		GPIO_PB5	//B5 -- cool white, led PWM5 mux 3
* LED_WW        GPIO_PC2    //C2 -  warm white      PWM0 mux 4
****************************************************************/

//#warning("Tuya CW/CCT support")
//PWM configuration, LED_WW as warm light, LED_CW as cool light or single LED.
#define LED_WW						GPIO_PC2	//C2 warm white     PWM0
#define LED_CW						GPIO_PB5	//B5 cool white		PWM5

#define PWM_WW_CHANNEL				0//PWM0
#define PWM_WW_CHANNEL_SET()		do{	\
										gpio_set_func(LED_WW, AS_PWM0); 		\
									}while(0)

#define PWM_CW_CHANNEL				5//PWM5
#define PWM_CW_CHANNEL_SET()		do{	\
										gpio_set_func(LED_CW, AS_PWM5); 	\
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_WW_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_CW_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_WW_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_CW_CHANNEL_SET()

//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PB4
#define LED_G						GPIO_PC3
#define LED_B						GPIO_PD2

#define PWM_R_CHANNEL               4//PWM4
#define PWM_R_CHANNEL_SET()         do{ \
                                        gpio_set_func(LED_R, AS_PWM4);      \
                                    }while(0)

#define PWM_G_CHANNEL               1//PWM1
#define PWM_G_CHANNEL_SET()         do{ \
                                        gpio_set_func(LED_G, AS_PWM1);  \
                                        /* drv_pwm_n_invert(PWM_G_CHANNEL);*/   \
                                    }while(0)

#define PWM_B_CHANNEL               3//PWM3
#define PWM_B_CHANNEL_SET()         do{ \
                                        gpio_set_func(LED_B, AS_PWM3);      \
                                    }while(0)

#define R_LIGHT_PWM_CHANNEL         PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL         PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL         PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()           PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()           PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()           PWM_B_CHANNEL_SET()

/*#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define PC3_FUNC					AS_GPIO
#define PC3_OUTPUT_ENABLE			1
#define PC3_INPUT_ENABLE			0

#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			1
#define PD2_INPUT_ENABLE			0*/

#define LED_POWER					NULL
#define LED_PERMIT					NULL


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
