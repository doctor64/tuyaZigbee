/********************************************************************************************************
 * @file    tuyaLight.h
 *
 * @brief   This is the header file for tuyaLight
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

#ifndef _TUYA_LIGHT_H_
#define _TUYA_LIGHT_H_



/**********************************************************************
 * CONSTANT
 */
#define TUYA_LIGHT_ENDPOINT   0x01
#define SAMPLE_TEST_ENDPOINT	0x02

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
	u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	bool bdbFindBindFlg;
	bool lightAttrsChanged;

	app_linkKey_info_t tcLinkKey;
}app_ctx_t;

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
	u8 	zclVersion;                         //attr 0
	u8	appVersion;                         //attr 1
	u8	stackVersion;                       //attr 2
	u8	hwVersion;                          //attr 3
	u8	manuName[ZCL_BASIC_MAX_LENGTH];     //attr 4
	u8	modelId[ZCL_BASIC_MAX_LENGTH];      //attr 5
	// u8  dateCode[ZCL_BASIC_MAX_LENGTH];     //attr 6
	u8	powerSource;                        //attr 7
	u8  genDevClass;                        //attr 8
	u8  genDevType;                         //attr 9
	u8	deviceEnable;                       //attr 12
	u8  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
}zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
	u8	nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
	u8	 sceneCount;
	u8	 currentScene;
	u8	 nameSupport;
	bool sceneValid;
	u16	 currentGroup;
}zcl_sceneAttr_t;

/**
 *  @brief Defined for on/off cluster attributes
 */
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
}zcl_onOffAttr_t;

/**
 *  @brief Defined for level cluster attributes
 */
typedef struct{
	u16	remainingTime;
	u8	curLevel;
	u8	startUpCurrentLevel;
}zcl_levelAttr_t;

/**
 *  @brief Defined for color control cluster attributes
 */
typedef struct{
	u8	colorMode;
	u8	options;
	u8	enhancedColorMode;
	u8	numOfPrimaries;
	u16 colorCapabilities;
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
	u8	colorLoopActive;
	u8	colorLoopDirection;
	u16	colorLoopTime;
	u16 colorLoopStartEnhancedHue;
	u16 colorLoopStoredEnhancedHue;
#elif COLOR_CCT_SUPPORT
	u16 colorTemperatureMireds;
	u16 colorTempPhysicalMinMireds;
	u16 colorTempPhysicalMaxMireds;
	u16 startUpColorTemperatureMireds;
#endif
}zcl_lightColorCtrlAttr_t;

/**
 *  @brief Defined for saving on/off attributes
 */
typedef struct {
	u8	onOff;
	u8	startUpOnOff;
}zcl_nv_onOff_t;

/**
 *  @brief Defined for saving level attributes
 */
typedef struct {
	u8	curLevel;
	u8	startUpCurLevel;
}zcl_nv_level_t;

/**
 *  @brief Defined for saving color control attributes
 */
typedef struct {
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
#elif COLOR_CCT_SUPPORT
	u16	colorTemperatureMireds;
	u16	startUpColorTemperatureMireds;
#endif
}zcl_nv_colorCtrl_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t gLightCtx;
extern bdb_commissionSetting_t g_bdbCommissionSetting;
extern bdb_appCb_t g_zbDemoBdbCb;


extern u8 TUYALIGHT_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_tuyaLightClusterList[];
extern const af_simple_descriptor_t tuyaLight_simpleDesc;
#if AF_TEST_ENABLE
extern const af_simple_descriptor_t sampleTestDesc;
#endif

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_groupAttr_t g_zcl_groupAttrs;
extern zcl_sceneAttr_t g_zcl_sceneAttrs;
extern zcl_onOffAttr_t g_zcl_onOffAttrs;
extern zcl_levelAttr_t g_zcl_levelAttrs;
extern zcl_lightColorCtrlAttr_t g_zcl_colorCtrlAttrs;

#define zcl_sceneAttrGet()		&g_zcl_sceneAttrs
#define zcl_onoffAttrGet()		&g_zcl_onOffAttrs
#define zcl_levelAttrGet()		&g_zcl_levelAttrs
#define zcl_colorAttrGet()		&g_zcl_colorCtrlAttrs

/**********************************************************************
 * FUNCTIONS
 */
void tuyaLight_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t tuyaLight_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaLight_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaLight_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaLight_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaLight_levelCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaLight_colorCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void tuyaLight_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void tuyaLight_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void tuyaLight_otaProcessMsgHandler(u8 evt, u8 status);
bool tuyaLight_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);

void tuyaLight_onoff(u8 cmd);

void zcl_tuyaLightAttrsInit(void);
nv_sts_t zcl_onOffAttr_save(void);
nv_sts_t zcl_levelAttr_save(void);
nv_sts_t zcl_colorCtrlAttr_save(void);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _TUYA_LIGHT_H_ */
