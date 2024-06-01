/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "../common/comm_cfg.h"
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "tuyaSwitch.h"
#include "app_ui.h"
#include "buttons.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin)
{
	//printf("LED%d on\n", pin);
    drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	//printf("LED%d off\n", pin);
    drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	//printf("Light on\n");
    led_on(LED1);
}

void light_off(void)
{
	//printf("Light off\n");
    led_off(LED1);
}

void light_init(void)
{
	//printf("Light Init\n");
    led_off(LED1);

}

s32 zclLightTimerCb(void *arg)
{
    u32 interval = 0;

    if(g_switchAppCtx.sta == g_switchAppCtx.oriSta){
        g_switchAppCtx.times--;
        if(g_switchAppCtx.times <= 0){
            g_switchAppCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_switchAppCtx.sta = !g_switchAppCtx.sta;
    if(g_switchAppCtx.sta){
        light_on();
        interval = g_switchAppCtx.ledOnTime;
    }else{
        light_off();
        interval = g_switchAppCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_switchAppCtx.times = times;

    if(!g_switchAppCtx.timerLedEvt){
        if(g_switchAppCtx.oriSta){
            light_off();
            g_switchAppCtx.sta = 0;
            interval = ledOffTime;
        }else{
            light_on();
            g_switchAppCtx.sta = 1;
            interval = ledOnTime;
        }
        g_switchAppCtx.ledOnTime = ledOnTime;
        g_switchAppCtx.ledOffTime = ledOffTime;
        //printf("timer4 led blink start \n");
        g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if(g_switchAppCtx.timerLedEvt){
        TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

        g_switchAppCtx.times = 0;
        if(g_switchAppCtx.oriSta){
            light_on();
        }else{
            light_off();
        }
    }
}

void cmdSendReport()
{
	printf("cmdSendReport\n");
    if(zb_isDeviceJoinedNwk()){
    	//light_blink_start(5, 500, 500);
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        /* fix for issue #21 - send report to coordinator only */
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = TUYA_SWITCH_ENDPOINT;
        dstEpInfo.dstAddr.shortAddr = 0x0; //send to coordinator
#endif
        //printf("Send Report\n");
    	zclAttrInfo_t *pAttrEntry;
    	pAttrEntry = zcl_findAttribute(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
    	zcl_sendReportCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
    			ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    	pAttrEntry = zcl_findAttribute(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
        zcl_sendReportCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
				ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    }
}
void cmdToggle(void)
{
	printf("cmdToggle\n");
	if(zb_isDeviceJoinedNwk())
	{
		//light_blink_start(1, 500, 0);
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
		dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        //printf("cmdToggle fill addr \n");
        /* fix for issue #21 - send report to binded only */
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT; /* send using data from binded table */
#endif
		zcl_onOff_toggleCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
	}
}

void cmdMoveOnOff(void)
{
	printf("cmdMoveOnOff\n");
	if(zb_isDeviceJoinedNwk())
	{
		//static u8 lvl = 1;
		static bool dir = 0;

		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        //printf("cmdMoveOnOff fill addr\n");
        /* fix for issue #21 - send report to binded only */
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT; /* send using data from binded table */
		dstEpInfo.profileId = HA_PROFILE_ID;

		//moveToLvl_t move2Level;

		//move2Level.optPresent = 0;
		//move2Level.transitionTime = 0x0A;
		//move2Level.level = lvl;

		//zcl_level_move2levelCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move2Level);
		move_t move;

		move.moveMode = dir;
		move.rate = 20;
		move.optPresent = 0;

		zcl_level_moveWithOnOffCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move);

		if(dir){
//                lvl += 50;
//                if(lvl >= 250){
            dir = 0;
//                }
		}else{
//                lvl -= 50;
//                if(lvl <= 1){
            dir = 1;
//                }
		}
	}
}
void cmdMove2Level(void)
{
    printf("cmdMove2Level\n");
    if(zb_isDeviceJoinedNwk())
    {
        static u8 lvl = 1;
        static bool dir = 1;

        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        //printf("cmdMoveOnOff fill addr\n");
        /* fix for issue #21 - send report to binded only */
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT; /* send using data from binded table */
        dstEpInfo.profileId = HA_PROFILE_ID;

        moveToLvl_t move2Level;

        move2Level.optPresent = 0;
        move2Level.transitionTime = 0x0A;
        move2Level.level = lvl;
printf("level:%d\n", lvl);
        zcl_level_move2levelCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move2Level);

        if (dir) {
            lvl += 50;
            if(lvl >= 250) {
                dir = 0;
            }
        } else {
            lvl -= 50;
            if (lvl <= 1) {
                dir = 1;
            }
        }
    }
}

void cmdStopWithOnOff(void)
{
	printf("cmdStop\n");
    if(zb_isDeviceJoinedNwk())  {
    	epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        /* fix for issue #21 - send report to binded only */
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT; /* send using data from binded table */
        dstEpInfo.profileId = HA_PROFILE_ID;

        stop_t stop;
        stop.optPresent = 0;

        zcl_level_stopWithOnOffCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &stop);
    }
}

s32 battVoltageCb(void *arg) {
	u16 voltage, percentage;
	u8 converted_voltage, percentage2;
	voltage = drv_get_adc_data();
	//printf("voltage %d\n", voltage);
	converted_voltage = (u8)(voltage/100);
	percentage = ((voltage - BATTERY_SAFETY_THRESHOLD)/4);
	if (percentage > 0xc8) percentage=0xc8;
	percentage2 = (u8)percentage;
	//printf("converted voltage %d diff %d", converted_voltage, (voltage - BATTERY_SAFETY_THRESHOLD));
	//printf(" , percentage2 %d\n", percentage2);
	zcl_setAttrVal(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &converted_voltage);
	zcl_setAttrVal(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);
	return 0;
}

/*******************************************************************
 * @brief   Button click detect:
 *          NET. keep press button1 5s === factory reset
 *          NET. short press button1   === send report
 *          SW1. click button2 === send OnOff togle
 *          SW1. keep press button2 3s === send Move (1st hold - up, 2nd hold - down)
 *          SW1. release button2 after long press  === send Stop (on release)
 *
 */

/**
 * @brief      Called on button long press
 *
 * @param[in]  button  The button number
 */
void buttonLongPress(u8 button) {
    printf("Long press button:%d\n", button);
    if(button == VK_NET) {
    	//printf("Button long pressed NET\n");
        g_switchAppCtx.buttons[button].state = BUTTON_STATE_PRESSED_PROCESSED;
    	light_blink_stop();
    	light_blink_start(255, 300, 300);
        //g_switchAppCtx.state = APP_FACTORY_NEW_DOING;
        zb_factoryReset();
        //not really sure it needed
        zb_resetDevice();
    }else if(button == VK_SW1) {
    	//printf("Button long pressed SW1\n");
        g_switchAppCtx.buttons[button].state = BUTTON_STATE_PRESSED_PROCESSED;
    	light_blink_stop();
    	light_blink_start(255, 200, 200);
    	cmdMoveOnOff();
    }
}
/**
 * @brief      Called on button long press release
 *
 * @param[in]  button  The button
 */
void buttonLongRelease(u8 button) {
    printf("Long press release button:%d\n", button);
    g_switchAppCtx.buttons[button].state = BUTTON_STATE_IDLE;
    g_switchAppCtx.buttons[button].count = 0;
    if(button == VK_NET) {
        //printf("Button long press NET release\n");
        light_blink_stop();
    }else if(button == VK_SW1) {
        //printf("Button long pressed SW1 release\n");
        cmdStopWithOnOff();
        light_blink_stop();
    }
}
/**
 * @brief      Called on button clicked (press+release)
 *
 * @param[in]  button  The button
 * @param[in]  count   The pressed count
 */
void buttonClick(u8 button, u8 count) {
    printf("Button:%d clicked num:%d\n", button, count);
    if(button == VK_NET) {
        if (count == 1) {
            //printf("Button single click NET\n");
            light_blink_start(2,200,0);
            cmdSendReport();
        }
    }else if(button == VK_SW1){
        if (count == 1) {
            //printf("Button single click SW1\n");
            light_blink_start(1, 300, 0);
            cmdToggle();
        } else if (count == 2) {
            light_blink_start(2, 300, 100);
            cmdMove2Level();
        }
    }
    //light_blink_stop();
}

void buttonPressed(u8 button) {
    printf("Pressed button:%d\n", button);
    if(button == VK_NET){
    	light_blink_start(5,300,700);
    	//cmdSendReport();
    } else if(button == VK_SW1) {
    	light_blink_start(1, 3000, 0);
    }
}

void buttonReleaseCheck(u8 button) {
    g_switchAppCtx.buttons[button].state = BUTTON_STATE_IDLE;
    buttonClick(button, g_switchAppCtx.buttons[button].count);
    g_switchAppCtx.buttons[button].count = 0;
}

/**
 * @brief      Process key presses
 *
 * @param      kbEvt  The keyboard event
 */
void keyScan_keyPressed(kb_data_t *kbEvt){
    u8 button = kbEvt->keycode[0];

    if (button != 0xff) {
        g_switchAppCtx.buttons[button].press_time = clock_time();
        g_switchAppCtx.buttons[button].state = BUTTON_STATE_PRESSED;
        g_switchAppCtx.buttons[button].count++;
        buttonPressed(button);
    }
}

/**
 * @brief      Process key releases
 *
 * @param[in]  keyCode  The key code
 */
void keyScan_keyReleased(u8 button) {
    if (button != 0xff) {
        g_switchAppCtx.buttons[button].release_time = clock_time();
        if (g_switchAppCtx.buttons[button].state == BUTTON_STATE_PRESSED_PROCESSED) { //Long press released
            g_switchAppCtx.buttons[button].state = BUTTON_STATE_LONG_RELEASED;
        } else {
            g_switchAppCtx.buttons[button].state = BUTTON_STATE_RELEASED;
        }
    }
}

void app_key_handler(void){
    static u8 valid_keyCode = 0xff;
    //static u8 keyPressed = 0;

    for (u8 i = 0; i < TOTAL_BUTTONS_NUM; i++) {
        if (g_switchAppCtx.buttons[i].state == BUTTON_STATE_PRESSED) {
            if (i == 0) { //special case for NET button
                if (clock_time_exceed(g_switchAppCtx.buttons[i].press_time, BUTTON_NET_LONG_PRESS_TIME)){
                    buttonLongPress(i);
                }
            }
            else {
                if (clock_time_exceed(g_switchAppCtx.buttons[i].press_time, BUTTON_SW_LONG_PRESS_TIME)){
                    buttonLongPress(i);
                }
            }
        }
        if (g_switchAppCtx.buttons[i].state == BUTTON_STATE_RELEASED) {
            if (clock_time_exceed(g_switchAppCtx.buttons[i].press_time, BUTTON_CLICK_CHECK_TIME)){
                buttonReleaseCheck(i);
            }
        }
        if (g_switchAppCtx.buttons[i].state == BUTTON_STATE_LONG_RELEASED) {
            buttonLongRelease(i);
        }

    }
    if(kb_scan_key(0, 1)){//if 1 keyboard update detected
        if(kb_event.cnt){ //number of keys currently pressed
            //printf("kb key pressed num:%d key code:0x%x\n", kb_event.cnt, kb_event.keycode[0]);
            g_switchAppCtx.keyPressed = 1;
            keyScan_keyPressed(&kb_event);
            if(kb_event.cnt == 1){
                valid_keyCode = kb_event.keycode[0];
            }
        }else{
            //printf("key released key code:0x%x\n", valid_keyCode);
            keyScan_keyReleased(valid_keyCode);
            valid_keyCode = 0xff;
            g_switchAppCtx.keyPressed = 0;
        }
    }
}

#endif  /* __PROJECT_TL_SWITCH__ */
