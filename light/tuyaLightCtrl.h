/********************************************************************************************************
 * @file    tuyaLightCtrl.h
 *
 * @brief   This is the header file for tuyaLightCtrl
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

#ifndef _TUYA_LIGHT_CTRL_H_
#define _TUYA_LIGHT_CTRL_H_


/**********************************************************************
 * CONSTANT
 */


/**********************************************************************
 * FUNCTIONS
 */
void hwLight_init(void);
void hwLight_onOffUpdate(u8 onOff);
void hwLight_levelUpdate(u8 level);
void hwLight_colorUpdate_colorTemperature(u16 colorTemperatureMireds, u8 level);
void hwLight_colorUpdate_HSV2RGB(u8 hue, u8 saturation, u8 level);

void light_adjust(void);
void light_fresh(void);
void light_applyUpdate(u8 *curLevel, u16 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u8 minLevel, u8 maxLevel, bool wrap);
void light_applyUpdate_16(u16 *curLevel, u32 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u16 minLevel, u16 maxLevel, bool wrap);

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);

#endif	/* _TUYA_LIGHT_CTRL_H_ */
