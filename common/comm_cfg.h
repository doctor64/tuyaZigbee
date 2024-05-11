/********************************************************************************************************
 * @file    comm_cfg.h
 *
 * @brief   This is the header file for comm_cfg
 *
 * @author  Zigbee Group
 * @date    2021
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

#define XSTR(x) STR(x)
#define STR(x) #x


/**********************************************************************
 * User configuration whether to use boot loader mode.
 *
 * NOTE:
 * We do not recommend using BootLoader mode on 512K flash chips,
 * because in boot loader mode, it can only support up to 196k firmware
 * according to the current default Flash Map.
 *
 * Please refer to the drv_nv.h file, and check if the firmware size
 * exceeds the maximum limit.
 *
 * The user can configure the CHIP_TYPE used by the corresponding
 * project in the version.h file according to the actual size
 * of the flash at hand.
 *
 * If BOOT_LOADER_MODE is 0, it means that the SDK uses Normal Mode,
 * hardware multi-address (0x0000 or 0x40000) startup mode.
 * If BOOT_LOADER_MODE is 1, it means that the SDK uses Boot Loader Mode.
 *
 * Normal mode is used by default.
 */
#define BOOT_LOADER_MODE                    1



/* Boot loader address. */
#define BOOT_LOADER_IMAGE_ADDR              0x0

/* APP image address. */
#if (BOOT_LOADER_MODE)
    #define APP_IMAGE_ADDR                  0x8000
#else
    #define APP_IMAGE_ADDR                  0x0
#endif


/* Chip IDs */
#define TLSR_8267                           0x00
#define TLSR_8269                           0x01
#define TLSR_8258_512K                      0x02
#define TLSR_8258_1M                        0x03
#define TLSR_8278                           0x04
#define TLSR_B91                            0x05

#if defined(MCU_CORE_826x)
    #if (CHIP_8269)
        #define CHIP_TYPE                   TLSR_8269
    #else
        #define CHIP_TYPE                   TLSR_8267
    #endif
#elif defined(MCU_CORE_8258)
        #define CHIP_TYPE                   TLSR_8258_1M//TLSR_8258_512K
#elif defined(MCU_CORE_8278)
        #define CHIP_TYPE                   TLSR_8278
#elif defined(MCU_CORE_B91)
        #define CHIP_TYPE                   TLSR_B91
#endif

/* Image types */
#if (BOOT_LOADER_MODE)
    #define IMAGE_TYPE_BOOT_FLAG            1
#else
    #define IMAGE_TYPE_BOOT_FLAG            0
#endif
 
// legacy values from SDK
#define IMAGE_TYPE_BOOTLOADER               (0xFF)
#define IMAGE_TYPE_GW                       (0x00 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT                    (0x01 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_SWITCH                   (0x02 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_CONTACT_SENSOR           (0x03 | (IMAGE_TYPE_BOOT_FLAG << 7))

#define IMAGE_TYPE_LIGHT_0501B              (0x11 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT_0502B              (0x12 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT_0503B              (0x13 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT_0504B              (0x14 | (IMAGE_TYPE_BOOT_FLAG << 7))
#define IMAGE_TYPE_LIGHT_0505B              (0x15 | (IMAGE_TYPE_BOOT_FLAG << 7))

#define IMAGE_TYPE_SWITCH_ZBWS01A           (0x21 | (IMAGE_TYPE_BOOT_FLAG << 7))

#define IMAGE_TYPE_IASZONE_SENSOR_ZG102ZL   (0x31 | (IMAGE_TYPE_BOOT_FLAG << 7))

/* Board ID */
#define BOARD_826x_EVK                       0
#define BOARD_826x_DONGLE                    1
#define BOARD_826x_DONGLE_PA                 2
#define BOARD_8258_EVK                       3
#define BOARD_8258_EVK_V1P2                  4//C1T139A30_V1.2
#define BOARD_8258_DONGLE                    5
#define BOARD_8278_EVK                       6
#define BOARD_8278_DONGLE                    7
#define BOARD_B91_EVK                        8
#define BOARD_B91_DONGLE                     9
#define BOARD_8258_TUYA_SWITCH_ZBWS01A      11
#define BOARD_8258_TUYA_SWITCH_ZBWS02A      12
#define BOARD_8258_TUYA_SWITCH_ZBWS03A      13
#define BOARD_8258_TUYA_SWITCH_ZBWS04A      14

#define BOARD_8258_TUYA_LIGHT_STRIP_TS0501B 30

#define BOARD_8258_TUYA_DOOR_SENSOR_ZG102ZL 60

/* Board define */
#if defined(MCU_CORE_826x)
#if !PA_ENABLE
    #define BOARD                       BOARD_826x_DONGLE
#else
    #define BOARD                       BOARD_826x_DONGLE_PA
#endif
    #define CLOCK_SYS_CLOCK_HZ          32000000
#elif defined(MCU_CORE_8258)
#if (CHIP_TYPE == TLSR_8258_1M)
    #define FLASH_CAP_SIZE_1M           1
#endif
    #if defined(BUILD_ZBWS01A)
        #define BOARD                       BOARD_8258_TUYA_SWITCH_ZBWS01A
    #elif defined(BUILD_ZBWS02A)
        #define BOARD                       BOARD_8258_TUYA_SWITCH_ZBWS02A
    #elif defined(BUILD_ZBWS03A)
        #define BOARD                       BOARD_8258_TUYA_SWITCH_ZBWS03A
    #elif defined(BUILD_ZBWS04A)
        #define BOARD                       BOARD_8258_TUYA_SWITCH_ZBWS04A
    #elif defined(BUILD_TS0501B)
        #define BOARD                       BOARD_8258_TUYA_LIGHT_STRIP_TS0501B
    #elif defined(BUILD_ZG102ZL)
        #define BOARD                       BOARD_8258_TUYA_DOOR_SENSOR_ZG102ZL
    #else
        #error "Build variant not defined"
    #endif
    #define CLOCK_SYS_CLOCK_HZ          48000000
#elif defined(MCU_CORE_8278)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_8278_DONGLE//BOARD_8278_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#elif defined(MCU_CORE_B91)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_B91_DONGLE//BOARD_B91_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#else
    #error "MCU is undefined!"
#endif

/* Board include */
#if (BOARD == BOARD_826x_EVK)
    #include "board_826x_evk.h"
#elif (BOARD == BOARD_826x_DONGLE)
    #include "board_826x_dongle.h"
#elif (BOARD == BOARD_826x_DONGLE_PA)
    #include "board_826x_dongle_pa.h"
#elif (BOARD == BOARD_8258_DONGLE)
    #include "board_8258_dongle.h"
#elif (BOARD == BOARD_8258_EVK)
    #include "board_8258_evk.h"
#elif (BOARD == BOARD_8258_EVK_V1P2)
    #include "board_8258_evk_v1p2.h"
#elif (BOARD == BOARD_8278_EVK)
    #include "board_8278_evk.h"
#elif (BOARD == BOARD_8278_DONGLE)
    #include "board_8278_dongle.h"
#elif (BOARD == BOARD_B91_EVK)
    #include "board_b91_evk.h"
#elif (BOARD == BOARD_B91_DONGLE)
    #include "board_b91_dongle.h"
#elif (BOARD == BOARD_8258_TUYA_SWITCH_ZBWS01A)
    #include "../device_config/switch_zbws01a.h"
    //#warning "tuya board"
#elif (BOARD == BOARD_8258_TUYA_LIGHT_STRIP_TS0501B)
    #include "../device_config/light_ts0501b.h"
#elif (BOARD == BOARD_8258_TUYA_DOOR_SENSOR_ZG102ZL)
    #include "../device_config/iaszone_zg102zl.h"

#else
    #error "Board is undefined"
#endif

//#warning "board BOARD"

// #pragma message "The value of CHIP_TYPE: " XSTR(CHIP_TYPE)
// #pragma message "The value of BOARD: " XSTR(BOARD)
// #pragma message "The value of FLASH_CAP_SIZE_1M: " XSTR(FLASH_CAP_SIZE_1M)