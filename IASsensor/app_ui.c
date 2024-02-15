/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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


#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "drivers/drv_adc.h"
#include "contactSensor.h"
#include "app_ui.h"
#include "zcl.h"
#include "security_safety/zcl_ias_zone.h"
#include "zcl_onoffSwitchCfg.h"
// #include "zb_af.h"

extern drv_pm_pinCfg_t g_sensorPmCfg[];

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
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	led_on(LED1);
}

void light_off(void)
{
	led_off(LED1);
}

void light_init(void)
{
	led_off(LED1);

}

s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(g_sensorAppCtx.sta == g_sensorAppCtx.oriSta){
		g_sensorAppCtx.times--;
		if(g_sensorAppCtx.times <= 0){
			g_sensorAppCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	g_sensorAppCtx.sta = !g_sensorAppCtx.sta;
	if(g_sensorAppCtx.sta){
		light_on();
		interval = g_sensorAppCtx.ledOnTime;
	}else{
		light_off();
		interval = g_sensorAppCtx.ledOffTime;
	}

	return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_sensorAppCtx.times = times;

	if(!g_sensorAppCtx.timerLedEvt){
		if(g_sensorAppCtx.oriSta){
			light_off();
			g_sensorAppCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			g_sensorAppCtx.sta = 1;
			interval = ledOnTime;
		}
		g_sensorAppCtx.ledOnTime = ledOnTime;
		g_sensorAppCtx.ledOffTime = ledOffTime;

		g_sensorAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
	}
}

void light_blink_stop(void)
{
	if(g_sensorAppCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&g_sensorAppCtx.timerLedEvt);

		g_sensorAppCtx.times = 0;
		if(g_sensorAppCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}
void cmdSendReport(void)
{
	//printf("cmdSendReport\n");
    if(zb_isDeviceJoinedNwk()){
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = CONTACT_SENSOR_ENDPOINT;
        dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif //FIND_AND_BIND_SUPPORT
    	zclAttrInfo_t *pAttrEntry;
    	pAttrEntry = zcl_findAttribute(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
    	zcl_sendReportCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
    			ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    	pAttrEntry = zcl_findAttribute(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
	zcl_sendReportCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
				ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
#ifdef HAVE_LIGHT_SENSOR
    	pAttrEntry = zcl_findAttribute(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, ZCL_ATTRID_MEASURED_VALUE);
	zcl_sendReportCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
				ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
#endif //HAVE_LIGHT_SENSOR
    }
}
void cmdSendOnOff(u8 sensor_state)
{
	u16 len;
	u8 switch_action;
	//light_blink_start(1, 500, 0);
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = CONTACT_SENSOR_ENDPOINT;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
	zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, ZCL_ATTRID_SWITCH_ACTION, &len, (u8*)&switch_action);
	printf("sw config %d action %d \n", switch_action, sensor_state);
	switch (switch_action) {
		case ZCL_SWITCH_ACTION_ON_OFF:
			if (sensor_state == APP_SENSOR_CLOSED)
				zcl_onOff_onCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
			if (sensor_state == APP_SENSOR_OPENED)
				zcl_onOff_offCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
			break;
		case ZCL_SWITCH_ACTION_OFF_ON:
			if (sensor_state == APP_SENSOR_CLOSED)
				zcl_onOff_offCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
			if (sensor_state == APP_SENSOR_OPENED)
				zcl_onOff_onCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, FALSE);			
			break;
		case ZCL_SWITCH_ACTION_TOGGLE:
			zcl_onOff_toggleCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
		};
	
}

/*******************************************************************
 * @brief	Button click detect:
 * 			SW1. keep press button1 5s === factory reset
 * 			SW1. short press button1   === send level step with OnOff command (Up)
 * 			SW2. keep press button2 5s === invoke EZ-Mode
 * 			SW2. short press button2   === send level step with OnOff command (Down)
 *
 */
void buttonKeepPressed(u8 btNum){
	if(btNum == VK_SW1){
		printf("Button keep pressed SW1\n");
		g_sensorAppCtx.state = APP_FACTORY_NEW_DOING;
		zb_factoryReset();
        //not really sure it needed
        zb_resetDevice();
	}
	else if(btNum == VK_SW2){
		printf("Button keep pressed SW2\n");

	}
}
void fillIASAddress(epInfo_t* pdstEpInfo)
{
	u16 len;
	u8 zoneState;

	memset((u8 *)pdstEpInfo, 0, sizeof(epInfo_t));

	zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &len, &zoneState);

	pdstEpInfo->dstEp = CONTACT_SENSOR_ENDPOINT;
	pdstEpInfo->profileId = HA_PROFILE_ID;

	if (zoneState&ZONE_STATE_ENROLLED) { //device enrolled
		pdstEpInfo->dstAddrMode = APS_LONG_DSTADDR_WITHEP;
		zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_IAS_CIE_ADDR, &len, (u8*)&pdstEpInfo->dstAddr.extAddr);
	}
	else {
		pdstEpInfo->dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		pdstEpInfo->dstAddr.shortAddr = 0x0000;
	}
}
void buttonShortPressed(u8 btNum){
	u16 len;
	epInfo_t dstEpInfo;
	zoneStatusChangeNoti_t statusChangeNotification;

	if(btNum == VK_SW1){
		printf("Button short pressed SW1\n");
		light_on();
    		//light_blink_start(5,300,700);
		if(zb_isDeviceJoinedNwk()){
			cmdSendReport();
			fillIASAddress(&dstEpInfo);

			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

			statusChangeNotification.zoneStatus |= ZONE_STATUS_TEST;
			zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
			statusChangeNotification.extStatus = 0;
			statusChangeNotification.delay = 0;

			zcl_iasZone_statusChangeNotificationCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
		}
	}else if(btNum == VK_SW2){
		printf("Button short pressed SW2 sensor closed\n");
    		light_blink_start(1, 100, 100);
		if(zb_isDeviceJoinedNwk()){
			fillIASAddress(&dstEpInfo);

			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

			statusChangeNotification.zoneStatus &= ~ZONE_STATUS_BIT_ALARM1;
			zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
			statusChangeNotification.extStatus = 0;
			statusChangeNotification.delay = 0;

			zcl_iasZone_statusChangeNotificationCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
#ifdef HAVE_ONOFF_SEND
			cmdSendOnOff(APP_SENSOR_CLOSED);
#endif
		}
	}
}


void keyScan_keyPressedCB(kb_data_t *kbEvt){
	//u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
	//static u8 lastKeyCode = 0xff;
	printf("keyScan_keyPressedCB keycode %d\n", keyCode);
	

	buttonShortPressed(keyCode);

	if(keyCode == VK_SW1){
		g_sensorAppCtx.keyPressedTime = clock_time();
		g_sensorAppCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
	if(keyCode == VK_SW2){
		g_sensorAppCtx.keyPressed = 0;
		drv_pm_wakeupPinLevelChange(g_sensorPmCfg, 2);
	}
}

void keyScan_keyReleasedCB(u8 keyCode){
	g_sensorAppCtx.state = APP_STATE_NORMAL;
	printf("keyScan_keyReleasedCB keycode %d\n", keyCode);
	u16 len;
	epInfo_t dstEpInfo;

	if(keyCode == VK_SW1) {
		light_off();
		if(zb_isDeviceJoinedNwk()) {
			fillIASAddress(&dstEpInfo);

			zoneStatusChangeNoti_t statusChangeNotification;

			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

			statusChangeNotification.zoneStatus &= ~ZONE_STATUS_TEST;
			zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
			statusChangeNotification.extStatus = 0;
			//statusChangeNotification.zoneId = ZCL_ZONE_ID_INVALID;
			statusChangeNotification.delay = 0;

			zcl_iasZone_statusChangeNotificationCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
		}
	}
	if(keyCode == VK_SW2) {
		light_blink_stop();

		drv_pm_wakeupPinLevelChange(g_sensorPmCfg, 2);
		if(zb_isDeviceJoinedNwk()) {
			fillIASAddress(&dstEpInfo);

			zoneStatusChangeNoti_t statusChangeNotification;

			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
			zcl_getAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

			statusChangeNotification.zoneStatus |= ZONE_STATUS_BIT_ALARM1;//ZONE_STATUS_TEST;
			zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
			statusChangeNotification.extStatus = 0;
			//statusChangeNotification.zoneId = ZCL_ZONE_ID_INVALID;
			statusChangeNotification.delay = 0;

			zcl_iasZone_statusChangeNotificationCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
#ifdef HAVE_ONOFF_SEND
			cmdSendOnOff(APP_SENSOR_OPENED);
#endif
		}
	}
}

void app_key_handler(void){
	static u8 valid_keyCode = 0xff;
	//printf("app_key_handler\n");
	if(g_sensorAppCtx.state == APP_FACTORY_NEW_SET_CHECK){
		if(clock_time_exceed(g_sensorAppCtx.keyPressedTime, 5*1000*1000)){
			buttonKeepPressed(VK_SW1);
		}
	}
	if(kb_scan_key(0 , 1)){
		if(kb_event.cnt){
			g_sensorAppCtx.keyPressed = 1;
			keyScan_keyPressedCB(&kb_event);
			if(kb_event.cnt == 1){
				valid_keyCode = kb_event.keycode[0];
			}
		}else{
			keyScan_keyReleasedCB(valid_keyCode);
			valid_keyCode = 0xff;
			g_sensorAppCtx.keyPressed = 0;
		}
	}
}

s32 battVoltageCb(void *arg) {
	u16 voltage, percentage;
	u8 converted_voltage, percentage2;

	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, VOLTAGE_DETECT_ADC_PIN);
	drv_adc_enable(1);
	voltage = drv_get_adc_data();
	printf("BAT voltage %d\n", voltage);
	converted_voltage = (u8)(voltage/100);
	percentage = ((voltage - BATTERY_SAFETY_THRESHOLD)/4);
	if (percentage > 0xc8) percentage=0xc8;
	percentage2 = (u8)percentage;
	//printf("converted voltage %d diff %d", converted_voltage, (voltage - BATTERY_SAFETY_THRESHOLD));
	//printf(" , percentage2 %d\n", percentage2);
	zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &converted_voltage);
	zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);
	drv_adc_enable(0);
	return 0;
}
s32 lightLevelCb(void *arg) {
	u16 voltageLux, voltageBatt, rawLuxInt, percentage;
	u8 converted_voltage, percentage2;
	u16 buf, adc, min, max;
	int i;

	//u32 resistance;
	//u16 rawLux;
	drv_adc_enable(0);
	//drv_adc_init();
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, LIGHT_SENSOR_SENSE);
	drv_adc_enable(1);
	//sleep_us(500000);
	buf = 0;
	min = 0xffff;
	max = 0;
	for (i=0; i<8; i++)
	{
		adc = drv_get_adc_data();
		if (max < adc) max = adc;
		if (min > adc) min = adc;
		//printf("ADC %d \n", adc);
		buf += adc;
	}

	voltageLux = buf / 8;
	drv_adc_enable(0);
	//drv_adc_init();
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, VOLTAGE_DETECT_ADC_PIN);
	drv_adc_enable(1);
	//sleep_us(500000);
	voltageBatt = drv_get_adc_data();
	printf("ADC voltage %d mV min %d max %d delta %d Batt voltage %d mV\n", voltageLux, min, max, max-min, voltageBatt );
	converted_voltage = (u8)(voltageBatt/100);
	percentage = ((voltageBatt - BATTERY_SAFETY_THRESHOLD)/4);
	if (percentage > 0xc8) percentage=0xc8;
	percentage2 = (u8)percentage;
	//printf("converted voltage %d diff %d", converted_voltage, (voltageBatt - BATTERY_SAFETY_THRESHOLD));
	//printf(" , percentage2 %d\n", percentage2);

	//resistance = (49000*(voltageBatt-voltageLux))/voltageLux;
	//rawLux = (-14900*(resistance)+79600);
	rawLuxInt=(u16)(28552*(voltageBatt - voltageLux)/voltageBatt);
	rawLuxInt=27934 - rawLuxInt;
	printf("rawLuxInt %d\n", rawLuxInt);
	//printf(" , percentage2 %d\n", percentage2);
	zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, ZCL_ATTRID_MEASURED_VALUE, (u8 *)&rawLuxInt);
	//zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);
	zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &converted_voltage);
	zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);

	//drv_adc_enable(0);
	//drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, VOLTAGE_DETECT_ADC_PIN);
	return 0;
}
#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
