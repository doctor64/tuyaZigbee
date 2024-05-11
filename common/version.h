/********************************************************************************************************
 * @file    version.h
 *
 * @brief   Contain common version definition for all subprojects
 *
 * @author  doctor64
 * @date    2024
 *******************************************************************************************************/
#pragma once

#include "comm_cfg.h"


#define APP_RELEASE                         0x10 //app release 1.0
#define APP_BUILD                           0x03 //app build 03
#define STACK_RELEASE                       0x30 //stack release 3.0
#define STACK_BUILD                         0x03 //stack build 03 - 3.6.8.6

#ifndef ZCL_BASIC_MFG_NAME
	#define ZCL_BASIC_MFG_NAME          {6,'D','o','c','L','a','b'}
#endif
#ifndef ZCL_BASIC_LOC_DESC
	#define ZCL_BASIC_LOC_DESC     {7,'U','N','K','N','O','W','N'}
#endif
#ifndef ZCL_BASIC_BUILD_ID
	#define ZCL_BASIC_BUILD_ID     {10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#ifndef ZCL_BASIC_SW_BUILD_ID //max 16 chars
	#define ZCL_BASIC_SW_BUILD_ID       {8,'v',(APP_RELEASE >> 4) + 0x30,'.',APP_BUILD + 0x30,'s',(STACK_RELEASE >> 4) + 0x30,'.',STACK_BUILD + 0x30}
#endif


/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */

// Note it is not real Telink ID, just to make SDK happy
#define MANUFACTURER_CODE_TELINK             0x6464 // updated manuf code //0x1141//Telink ID

//#define    IMAGE_TYPE                        0xD3A3 //for tuya OTA
#define    FILE_VERSION                      ((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)
//#define    FILE_VERSION                      0xff //for tuya ota
                                              //0x10023001
//OTA image type, define device type
//defined in device config
//#define IMAGE_TYPE                          ((CHIP_TYPE << 8) | IMAGE_TYPE_SWITCH_ZBWS01A) //0x03B1

