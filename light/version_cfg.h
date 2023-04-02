/********************************************************************************************************
 * @file    version_cfg.h
 *
 * @brief   This is the header file for version_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *            All rights reserved.
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

#include "../common/comm_cfg.h"

#if defined(MCU_CORE_826x)
    #if (CHIP_8269)
        #define CHIP_TYPE                    TLSR_8269
    #else
        #define CHIP_TYPE                    TLSR_8267
    #endif
#elif defined(MCU_CORE_8258)
        #define CHIP_TYPE                    TLSR_8258_1M //TLSR_8258_512K//TLSR_8258_1M
#elif defined(MCU_CORE_8278)
        #define CHIP_TYPE                    TLSR_8278
#elif defined(MCU_CORE_B91)
        #define CHIP_TYPE                    TLSR_B91
#endif

#define APP_RELEASE                          0x10//app release 1.0
#define APP_BUILD                            0x03//app build 01
#define STACK_RELEASE                        0x30//stack release 3.0
#define STACK_BUILD                          0x01//stack build 01
#define HARDWARE_REV                         0x01

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME          {6,'D','o','c','L','a','b'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
    #if defined BUILD_TS0501B
        #define ZCL_BASIC_MODEL_ID          {13,'F','i','x','e','d','_','T','S','0','5','0','1','B'} //TS0501B - CW
        #define COLOR_RGB_SUPPORT             0
        #define COLOR_CCT_SUPPORT             0
    #elif defined BUILD_TS0502B
        #define ZCL_BASIC_MODEL_ID          {13,'F','i','x','e','d','_','T','S','0','5','0','2','B'} //TS0502B - CW/WW
        #define COLOR_RGB_SUPPORT             0
        #define COLOR_CCT_SUPPORT             1
    #elif defined BUILD_TS0503B
        #define ZCL_BASIC_MODEL_ID          {13,'F','i','x','e','d','_','T','S','0','5','0','3','B'} //TS0503B - RGB
        #define COLOR_RGB_SUPPORT             1
        #define COLOR_CCT_SUPPORT             0
    #elif defined BUILD_TS0504B
        #define ZCL_BASIC_MODEL_ID          {13,'F','i','x','e','d','_','T','S','0','5','0','4','B'} //TS0504B - RGBW
        #define COLOR_RGB_SUPPORT             1
        #define COLOR_CCT_SUPPORT             0
    #elif defined BUILD_TS0505B
        #define ZCL_BASIC_MODEL_ID          {13,'F','i','x','e','d','_','T','S','0','5','0','5','B'} //TS0505B - RGB/CW/WW
        #define COLOR_RGB_SUPPORT             1
        #define COLOR_CCT_SUPPORT             1
    #else
        #warning "Unknown build variant"
    #endif
#endif

#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID       {8,'v',(APP_RELEASE >> 4) + 0x30,'.',APP_BUILD + 0x30,'s',(STACK_RELEASE >> 4) + 0x30,'.',STACK_BUILD + 0x30}
#endif

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define MANUFACTURER_CODE_DOCTOR64           0x6464
#define MANUFACTURER_CODE_TELINK             0x1141//Telink ID
#define    IMAGE_TYPE                        ((CHIP_TYPE << 8) | IMAGE_TYPE_LIGHT)
//#define    IMAGE_TYPE                        0xD3A3 //for tuya OTA
#define    FILE_VERSION                      ((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)
//#define    FILE_VERSION                      0xff //for tuya ota

/* Pre-compiled link configuration. */
#define IS_BOOT_LOADER_IMAGE                 0
#define RESV_FOR_APP_RAM_CODE_SIZE           0
#define IMAGE_OFFSET                         APP_IMAGE_ADDR
