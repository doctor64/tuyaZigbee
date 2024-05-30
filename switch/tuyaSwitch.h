/********************************************************************************************************
 * @file    tuyaSwitch.h
 *
 * @brief   This is the header file for tuyaSwitch
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

#ifndef _TUYA_SWITCH_H_
#define _TUYA_SWITCH_H_

#include "buttons.h"
#include "types.h"
#include "os/ev_timer.h"

/**********************************************************************
 * CONSTANT
 */
#define TUYA_SWITCH_ENDPOINT  0x01

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

typedef struct{
	ev_timer_event_t *bdbFBTimerEvt;
	ev_timer_event_t *timerLedEvt;
	ev_timer_event_t *timerBattEvt;
    u16 Vbat;		//current voltage
	//u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	u8  keyPressed;
	//u8  btn1State;
	button_t buttons[TOTAL_BUTTONS_NUM];

	app_linkKey_info_t tcLinkKey;
}app_ctx_t;

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
	u8 	zclVersion;
	u8	appVersion;
	u8	stackVersion;
	u8	hwVersion;
	u8	manuName[ZCL_BASIC_MAX_LENGTH];
	u8	modelId[ZCL_BASIC_MAX_LENGTH];
	//u8  dateCode[ZCL_BASIC_MAX_LENGTH];
	u8	powerSource;
	u8  genDevClass;                        //attr 8
	u8  genDevType;                         //attr 9
	u8	deviceEnable;
	u8  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
}zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for power configuration cluster attributes
 */
typedef struct{
#ifdef POWER_MAINS
	u16 mainsVoltage;
	u8  mainsFrequency;
#endif
	u8  batteryVoltage;      //0x20
	u8  batteryPercentage;   //0x21
}zcl_powerAttr_t;


/**
 *  @brief  Defined for poll control cluster attributes
 */
typedef struct{
	u32	chkInInterval;
	u32	longPollInterval;
	u32	chkInIntervalMin;
	u32	longPollIntervalMin;
	u16	shortPollInterval;
	u16	fastPollTimeout;
	u16	fastPollTimeoutMax;

	u32 chkBattInterval;
}zcl_pollCtrlAttr_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_switchAppCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

extern u8 TUYA_SWITCH_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_tuyaSwitchClusterList[];
extern const af_simple_descriptor_t tuyaSwitch_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs;

#define zcl_pollCtrlAttrGet()	&g_zcl_pollCtrlAttrs
/**********************************************************************
 * FUNCTIONS
 */
void tuyaSwitch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t tuyaSwitch_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaSwitch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaSwitch_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaSwitch_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaSwitch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaSwitch_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
void tuyaSwitch_zclCheckInStart(void);
void tuyaSwitch_zclBatterySensorRate(void);

void tuyaSwitch_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void tuyaSwitch_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void tuyaSwitch_otaProcessMsgHandler(u8 evt, u8 status);

#endif /* _TUYA_SWITCH_H_ */
