/********************************************************************************************************
 * @file    tuyaLightCtrl.c
 *
 * @brief   This is the source file for tuyaLightCtrl
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

#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "tuyaLight.h"
#include "tuyaLightCtrl.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */
#define PWM_FREQUENCY					1000	//1KHz
#define PWM_FULL_DUTYCYCLE				100
#define PMW_MAX_TICK		            (PWM_CLOCK_SOURCE / PWM_FREQUENCY)


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */
extern void tuyaLight_updateOnOff(void);
extern void tuyaLight_updateLevel(void);
extern void tuyaLight_updateColor(void);

extern void tuyaLight_onOffInit(void);
extern void tuyaLight_levelInit(void);
extern void tuyaLight_colorInit(void);

/*********************************************************************
 * @fn      pwmSetDuty
 *
 * @brief
 *
 * @param   ch			-	PWM channel
 * 			dutycycle	-	level * PWM_FULL_DUTYCYCLE
 *
 * @return  None
 */
void pwmSetDuty(u8 ch, u16 dutycycle)
{
#ifdef ZCL_LEVEL_CTRL
	u32 cmp_tick = ((u32)dutycycle * PMW_MAX_TICK) / (ZCL_LEVEL_ATTR_MAX_LEVEL * PWM_FULL_DUTYCYCLE);
	drv_pwm_cfg(ch, (u16)cmp_tick, PMW_MAX_TICK);
#endif
}

/*********************************************************************
 * @fn      pwmInit
 *
 * @brief
 *
 * @param   ch			-	PWM channel
 * 			dutycycle	-	level * PWM_FULL_DUTYCYCLE
 *
 * @return  None
 */
void pwmInit(u8 ch, u16 dutycycle)
{
	pwmSetDuty(ch, dutycycle);
}

/*********************************************************************
 * @fn      hwLight_init
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void hwLight_init(void)
{
	drv_pwm_init();

#if COLOR_RGB_SUPPORT
	R_LIGHT_PWM_SET();
	G_LIGHT_PWM_SET();
	B_LIGHT_PWM_SET();
	pwmInit(R_LIGHT_PWM_CHANNEL, 0);
	pwmInit(G_LIGHT_PWM_CHANNEL, 0);
	pwmInit(B_LIGHT_PWM_CHANNEL, 0);
#else
	COOL_LIGHT_PWM_SET();
	pwmInit(COOL_LIGHT_PWM_CHANNEL, 0);
#if COLOR_CCT_SUPPORT
	WARM_LIGHT_PWM_SET();
	pwmInit(WARM_LIGHT_PWM_CHANNEL, 0);
#endif
#endif
}

/*********************************************************************
 * @fn      hwLight_onOffUpdate
 *
 * @brief
 *
 * @param   onOff - onOff attribute value
 *
 * @return  None
 */
void hwLight_onOffUpdate(u8 onOff)
{
	if(onOff){
#if COLOR_RGB_SUPPORT
		drv_pwm_start(R_LIGHT_PWM_CHANNEL);
		drv_pwm_start(G_LIGHT_PWM_CHANNEL);
		drv_pwm_start(B_LIGHT_PWM_CHANNEL);
#else
#if COLOR_CCT_SUPPORT
		drv_pwm_start(WARM_LIGHT_PWM_CHANNEL);
#endif
		drv_pwm_start(COOL_LIGHT_PWM_CHANNEL);
#endif
	}else{
#if COLOR_RGB_SUPPORT
		drv_pwm_stop(R_LIGHT_PWM_CHANNEL);
		drv_pwm_stop(G_LIGHT_PWM_CHANNEL);
		drv_pwm_stop(B_LIGHT_PWM_CHANNEL);
#else
#if COLOR_CCT_SUPPORT
		drv_pwm_stop(WARM_LIGHT_PWM_CHANNEL);
#endif
		drv_pwm_stop(COOL_LIGHT_PWM_CHANNEL);
#endif
	}
}

/*********************************************************************
 * @fn      hwLight_levelUpdate
 *
 * @brief
 *
 * @param   level - level attribute value
 *
 * @return  None
 */
void hwLight_levelUpdate(u8 level)
{
#if !defined COLOR_RGB_SUPPORT || (COLOR_RGB_SUPPORT == 0)
	level = (level < 0x10) ? 0x10 : level;

	u16 gammaCorrectLevel = ((u16)level * level) / ZCL_LEVEL_ATTR_MAX_LEVEL;

	pwmSetDuty(COOL_LIGHT_PWM_CHANNEL, gammaCorrectLevel * PWM_FULL_DUTYCYCLE);
#endif
}

/*********************************************************************
 * @fn      temperatureToCW
 *
 * @brief
 *
 * @param   [in]colorTemperatureMireds	-	colorTemperatureMireds attribute value
 * 			[in]level					-	level attribute value
 * 			[out]C						-	cool light PWM
 * 			[out]W						-	warm light PWM
 *
 * @return  None
 */
void temperatureToCW(u16 temperatureMireds, u8 level, u8 *C, u8 *W)
{
#if COLOR_CCT_SUPPORT
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	*W = (u8)(((temperatureMireds - pColor->colorTempPhysicalMinMireds) * level) / (pColor->colorTempPhysicalMaxMireds - pColor->colorTempPhysicalMinMireds));
	*C = level - (*W);
#endif
}

/*********************************************************************
 * @fn      hwLight_colorUpdate_colorTemperature
 *
 * @brief
 *
 * @param   colorTemperatureMireds	-	colorTemperatureMireds attribute value
 * 			level					-	level attribute value
 *
 * @return  None
 */
void hwLight_colorUpdate_colorTemperature(u16 colorTemperatureMireds, u8 level)
{
#if COLOR_CCT_SUPPORT
	u8 C = 0;
	u8 W = 0;

	level = (level < 0x10) ? 0x10 : level;

	temperatureToCW(colorTemperatureMireds, level, &C, &W);

	u16 gammaCorrectC = ((u16)C * C) / ZCL_LEVEL_ATTR_MAX_LEVEL;
	u16 gammaCorrectW = ((u16)W * W) / ZCL_LEVEL_ATTR_MAX_LEVEL;

	pwmSetDuty(COOL_LIGHT_PWM_CHANNEL, gammaCorrectC * PWM_FULL_DUTYCYCLE);
	pwmSetDuty(WARM_LIGHT_PWM_CHANNEL, gammaCorrectW * PWM_FULL_DUTYCYCLE);
#endif
}

/*********************************************************************
 * @fn      hsvToRGB
 *
 * @brief
 *
 * @param   [in]hue			-	hue attribute value
 * 			[in]saturation	-	saturation attribute value
 * 			[in]level		-	level attribute value
 * 			[out]R			-	R light PWM
 * 			[out]G			-	G light PWM
 * 			[out]B			-	B light PWM
 *
 * @return  None
 */
void hsvToRGB(u8 hue, u8 saturation, u8 level, u8 *R, u8 *G, u8 *B)
{
#if COLOR_RGB_SUPPORT
    u8 region;
    u8 remainder;
    u8 p, q, t;

	u16 rHue = (u16)hue * 360 / ZCL_COLOR_ATTR_HUE_MAX;
	u8 rS = saturation;
	u8 rV = level;

	if(saturation == 0){
		*R = rV;
		*G = rV;
		*B = rV;
		return;
	}

	if(rHue < 360){
		region = rHue / 60;
	}else{
		region = 0;
	}

	remainder = (rHue - (region * 60)) * 4;

    p = (rV * (255 - rS)) >> 8;
    q = (rV * (255 - ((rS * remainder) >> 8))) >> 8;
    t = (rV * (255 - ((rS * (255 - remainder)) >> 8))) >> 8;

    if (region == 0) {
    	*R = rV;
    	*G = t;
    	*B = p;
    } else if (region == 1) {
    	*R = q;
    	*G = rV;
    	*B = p;
    } else if (region == 2) {
    	*R = p;
    	*G = rV;
    	*B = t;
    } else if (region == 3) {
    	*R = p;
    	*G = q;
    	*B = rV;
    } else if (region == 4) {
    	*R = t;
    	*G = p;
    	*B = rV;
    } else {
    	*R = rV;
    	*G = p;
    	*B = q;
    }
#endif
}

/*********************************************************************
 * @fn      hwLight_colorUpdate_HSV2RGB
 *
 * @brief
 *
 * @param   hue			-	hue attribute value
 * 			saturation	-	saturation attribute value
 * 			level		-	level attribute value
 *
 * @return  None
 */
void hwLight_colorUpdate_HSV2RGB(u8 hue, u8 saturation, u8 level)
{
#if COLOR_RGB_SUPPORT
	u8 R = 0;
	u8 G = 0;
	u8 B = 0;

	level = (level < 0x10) ? 0x10 : level;

	hsvToRGB(hue, saturation, level, &R, &G, &B);

	u16 gammaCorrectR = ((u16)R * R) / ZCL_LEVEL_ATTR_MAX_LEVEL;
	u16 gammaCorrectG = ((u16)G * G) / ZCL_LEVEL_ATTR_MAX_LEVEL;
	u16 gammaCorrectB = ((u16)B * B) / ZCL_LEVEL_ATTR_MAX_LEVEL;

	pwmSetDuty(PWM_R_CHANNEL, gammaCorrectR * PWM_FULL_DUTYCYCLE);
	pwmSetDuty(PWM_G_CHANNEL, gammaCorrectG * PWM_FULL_DUTYCYCLE);
	pwmSetDuty(PWM_B_CHANNEL, gammaCorrectB * PWM_FULL_DUTYCYCLE);
#endif
}

/*********************************************************************
 * @fn      light_adjust
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_adjust(void)
{
#ifdef ZCL_LIGHT_COLOR_CONTROL
	tuyaLight_colorInit();
#else
#ifdef ZCL_LEVEL_CTRL
	tuyaLight_levelInit();
#endif
#endif
	tuyaLight_onOffInit();
}

/*********************************************************************
 * @fn      light_fresh
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_fresh(void)
{
#ifdef ZCL_LIGHT_COLOR_CONTROL
	tuyaLight_updateColor();
#else
#ifdef ZCL_LEVEL_CTRL
	tuyaLight_updateLevel();
#else
	pwmSetDuty(COOL_LIGHT_PWM_CHANNEL, ZCL_LEVEL_ATTR_MAX_LEVEL * PWM_FULL_DUTYCYCLE);
#endif
#endif
	tuyaLight_updateOnOff();

	gLightCtx.lightAttrsChanged = TRUE;
}

/*********************************************************************
 * @fn      light_applyUpdate
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate(u8 *curLevel, u16 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u8 minLevel, u8 maxLevel, bool wrap)
{
	if((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)){
		*curLevel256 = (wrap) ? ((u16)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u16)maxLevel * 256) - 256)
							  : ((u16)maxLevel * 256);
	}else if((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)){
		*curLevel256 = (wrap) ? ((u16)maxLevel * 256 - ((u16)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
							  : ((u16)minLevel * 256);
	}else{
		*curLevel256 += *stepLevel256;
	}

	if(*stepLevel256 > 0){
		*curLevel = (*curLevel256 + 127) / 256;
	}else{
		*curLevel = *curLevel256 / 256;
	}

	if(*remainingTime == 0){
		*curLevel256 = ((u16)*curLevel) * 256;
		*stepLevel256 = 0;
	}else if(*remainingTime != 0xFFFF){
		*remainingTime = *remainingTime -1;
	}

	light_fresh();
}

/*********************************************************************
 * @fn      light_applyUpdate_16
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate_16(u16 *curLevel, u32 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u16 minLevel, u16 maxLevel, bool wrap)
{
	if((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)){
		*curLevel256 = (wrap) ? ((u32)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u32)maxLevel * 256) - 256)
							  : ((u32)maxLevel * 256);
	}else if((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)){
		*curLevel256 = (wrap) ? ((u32)maxLevel * 256 - ((u32)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
							  : ((u32)minLevel * 256);
	}else{
		*curLevel256 += *stepLevel256;
	}

	if(*stepLevel256 > 0){
		*curLevel = (*curLevel256 + 127) / 256;
	}else{
		*curLevel = *curLevel256 / 256;
	}

	if(*remainingTime == 0){
		*curLevel256 = ((u32)*curLevel) * 256;
		*stepLevel256 = 0;
	}else if(*remainingTime != 0xFFFF){
		*remainingTime = *remainingTime -1;
	}

	light_fresh();
}

/*********************************************************************
 * @fn      light_blink_TimerEvtCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
s32 light_blink_TimerEvtCb(void *arg)
{
	u32 interval = 0;

	if(gLightCtx.sta == gLightCtx.oriSta){
		if(gLightCtx.times){
			gLightCtx.times--;
			if(gLightCtx.times <= 0){
				if(gLightCtx.oriSta){
					hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
				}else{
					hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
				}

				gLightCtx.timerLedEvt = NULL;
				return -1;
			}
		}
	}

	gLightCtx.sta = !gLightCtx.sta;
	if(gLightCtx.sta){
		hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
		interval = gLightCtx.ledOnTime;
	}else{
		hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
		interval = gLightCtx.ledOffTime;
	}

	return interval;
}

/*********************************************************************
 * @fn      light_blink_start
 *
 * @brief
 *
 * @param   times 		- counts
 * @param   ledOnTime	- on times, ms
 * @param   ledOffTime	- off times, ms
 *
 * @return  None
 */
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	zcl_onOffAttr_t *pOnoff = zcl_onoffAttrGet();

	gLightCtx.oriSta = pOnoff->onOff;
	gLightCtx.times = times;

	if(!gLightCtx.timerLedEvt){
		if(gLightCtx.oriSta){
			hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
			gLightCtx.sta = 0;
			interval = ledOffTime;
		}else{
			hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
			gLightCtx.sta = 1;
			interval = ledOnTime;
		}
		gLightCtx.ledOnTime = ledOnTime;
		gLightCtx.ledOffTime = ledOffTime;

		gLightCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(light_blink_TimerEvtCb, NULL, interval);
	}
}

/*********************************************************************
 * @fn      light_blink_stop
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_blink_stop(void)
{
	if(gLightCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&gLightCtx.timerLedEvt);

		gLightCtx.times = 0;
		if(gLightCtx.oriSta){
			hwLight_onOffUpdate(ZCL_CMD_ONOFF_ON);
		}else{
			hwLight_onOffUpdate(ZCL_CMD_ONOFF_OFF);
		}
	}
}

#endif	/* __PROJECT_TL_DIMMABLE_LIGHT__ */
