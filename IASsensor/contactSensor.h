/********************************************************************************************************
 * @file    contactSensor.h
 *
 * @brief   This is the header file for contactSensor
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

#ifndef _CONTACT_CONTACT_SENSOR_H_
#define _CONTACT_CONTACT_SENSOR_H_


/**********************************************************************
 * CONSTANT
 */
#include "general/zcl_basic.h"
#define CONTACT_SENSOR_ENDPOINT  0x01

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

typedef struct{
	ev_timer_event_t *timerLedEvt;
	ev_timer_event_t *timerBattEvt;
	ev_timer_event_t *timerLightLevelEvt;
	u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	u8  keyPressed;

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
	u8  dateCode[ZCL_BASIC_MAX_LENGTH];
	u8	powerSource;
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
 *  @brief Defined for ias zone cluster attributes
 */
typedef struct {
	u8  zoneState;
	u16 zoneType;
	u16 zoneStatus;
	extAddr_t  iasCieAddr;
	u8  zoneId;
}zcl_iasZoneAttr_t;

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
	u32 chkLightInterval;
}zcl_pollCtrlAttr_t;

/**
 *  @brief  Defined for illuminance measurement cluster attributes
 */
typedef struct{
	u16	measuredValue;
	u16	minMeasuredValue;
	u16	maxMeasuredValue;
	u16	tolerance;
	u8	lightSensorType;
}zcl_illuminanceAttr_t;

typedef struct{
	u8 switchType;
	u8 switchActions;
}zcl_onOffSwitchCfg;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_sensorAppCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

extern u8 CONTACT_SENSOR_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_contactSensorClusterList[];
extern const af_simple_descriptor_t contactSensor_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_iasZoneAttr_t g_zcl_iasZoneAttrs;
extern zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs;

#define zcl_iasZoneAttrGet()	&g_zcl_iasZoneAttrs
#define zcl_pollCtrlAttrGet()	&g_zcl_pollCtrlAttrs

/**********************************************************************
 * FUNCTIONS
 */
void contactSensor_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t contactSensor_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t contactSensor_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t contactSensor_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t contactSensor_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t contactSensor_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t contactSensor_illuminanceCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
void contactSensor_zclCheckInStart(void);
void contactSensor_zclLightSensorRate(void);

void contactSensor_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void contactSensor_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void contactSensor_otaProcessMsgHandler(u8 evt, u8 status);


#endif /* _CONTACT_CONTACT_SENSOR_H_ */
