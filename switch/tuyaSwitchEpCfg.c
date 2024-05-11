/********************************************************************************************************
 * @file    tuyaSwitchEpCfg.c
 *
 * @brief   This is the source file for tuyaSwitchEpCfg
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

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "../common/comm_cfg.h"

#include "tl_common.h"
#include "zcl_include.h"
#include "tuyaSwitch.h"

#include "zcl_sensorControl.h"
#include "timestamp.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS  0x0008
#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE   0x0009


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 tuyaSwitch_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
	ZCL_CLUSTER_GEN_POWER_CFG,
#ifdef ZCL_POLL_CTRL
	ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 tuyaSwitch_outClusterList[] =
{
#ifdef ZCL_GROUP
	ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
	ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_LEVEL_CTRL
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
	ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define TUYASWITCH_IN_CLUSTER_NUM		(sizeof(tuyaSwitch_inClusterList)/sizeof(tuyaSwitch_inClusterList[0]))
#define TUYASWITCH_OUT_CLUSTER_NUM	(sizeof(tuyaSwitch_outClusterList)/sizeof(tuyaSwitch_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t tuyaSwitch_simpleDesc =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                	/* Application device identifier */
	TUYA_SWITCH_ENDPOINT,                 /* Endpoint */
	2,                                  	/* Application device version */
	0,										/* Reserved */
	TUYASWITCH_IN_CLUSTER_NUM,           	/* Application input cluster count */
	TUYASWITCH_OUT_CLUSTER_NUM,          	/* Application output cluster count */
	(u16 *)tuyaSwitch_inClusterList,    	/* Application input cluster list */
	(u16 *)tuyaSwitch_outClusterList,   	/* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
	.zclVersion 	= 0x03,//ZCL v7
	.appVersion 	= (APP_RELEASE|APP_BUILD),
	.stackVersion 	= (STACK_RELEASE|STACK_BUILD),
	.hwVersion		= HARDWARE_REV,
	.manuName		= ZCL_BASIC_MFG_NAME,
	.modelId		= ZCL_BASIC_MODEL_ID,
	//.dateCode       = ZCL_BASIC_DATE_CODE,
	.powerSource	= POWER_SOURCE_BATTERY,
	.genDevClass    = 0x00,  //Lightning
	.genDevType     = 0xe1,  //Wall Switch
	.swBuildId		= ZCL_BASIC_SW_BUILD_ID,
	.deviceEnable	= TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
	{ ZCL_ATTRID_BASIC_ZCL_VER,      		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.zclVersion},
	{ ZCL_ATTRID_BASIC_APP_VER,      		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.appVersion},
	{ ZCL_ATTRID_BASIC_STACK_VER,    		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.stackVersion},
	{ ZCL_ATTRID_BASIC_HW_VER,       		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.hwVersion},
	{ ZCL_ATTRID_BASIC_MFR_NAME,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)g_zcl_basicAttrs.manuName},
	{ ZCL_ATTRID_BASIC_MODEL_ID,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)g_zcl_basicAttrs.modelId},
	{ ZCL_ATTRID_BASIC_DATE_CODE,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)&build_time_str},//g_zcl_basicAttrs.dateCode},
	{ ZCL_ATTRID_BASIC_POWER_SOURCE, 		ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.powerSource},
	{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS,ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevClass},
	{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE, ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevType},
	{ ZCL_ATTRID_BASIC_DEV_ENABLED,  		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},
	{ ZCL_ATTRID_BASIC_SW_BUILD_ID,  		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.swBuildId},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_BASIC_ATTR_NUM		 sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
	.identifyTime	= 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
	{ ZCL_ATTRID_IDENTIFY_TIME,  			ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs.identifyTime },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_IDENTIFY_ATTR_NUM			sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

/* power */
zcl_powerAttr_t g_zcl_powerAttrs =
{
    .batteryVoltage    = 20, //in 1000 mV units, 0xff - unknown
    .batteryPercentage = 0x64, //in 0,5% units, 0xff - unknown
};

const zclAttrInfo_t powerCfg_attrTbl[] =
{
	{ ZCL_ATTRID_BATTERY_VOLTAGE,      		   ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,	(u8*)&g_zcl_powerAttrs.batteryVoltage},
	{ ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_powerAttrs.batteryPercentage},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_POWER_CFG_ATTR_NUM		 sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)


#ifdef ZCL_POLL_CTRL
/* Poll Control */
zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs =
{
	// default values from ZCL 7
	.chkInInterval			= POLL_CTL_DEFAULT_CHECKIN_INTERVAL,       // 1 hour
	.longPollInterval		= POLL_CTL_DEFAULT_LONG_POLL_INTERVAL,     // 5 sec
	.shortPollInterval		= POLL_CTL_DEFAULT_SHORT_POLL_INTERVAL,    // 2 qs = 0.5 sec
	.fastPollTimeout		= POLL_CTL_DEFAULT_FAST_POLL_INTERVAL,     // 10 sec
	.chkInIntervalMin		= 0x00,
	.longPollIntervalMin	= 0x00,
	.fastPollTimeoutMax		= 0x00,

	.chkBattInterval        = POLL_CTL_DEFAULT_BATTERY_CHECK_INTERVAL, // 5000 ms
};

const zclAttrInfo_t pollCtrl_attrTbl[] =
{
	{ ZCL_ATTRID_CHK_IN_INTERVAL,  		ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.chkInInterval },
	{ ZCL_ATTRID_LONG_POLL_INTERVAL, 	ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.longPollInterval },
	{ ZCL_ATTRID_SHORT_POLL_INTERVAL, 	ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.shortPollInterval },
	{ ZCL_ATTRID_FAST_POLL_TIMEOUT, 	ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeout },
	{ ZCL_ATTRID_CHK_IN_INTERVAL_MIN, 	ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.chkInIntervalMin},
	{ ZCL_ATTRID_LONG_POLL_INTERVAL_MIN,ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.longPollIntervalMin },
	{ ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeoutMax},

    { ZCL_ATTRID_SC_BATTERY_VOLTAGE_RATE, ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.chkBattInterval},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  					  (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_POLLCTRL_ATTR_NUM			sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_tuyaSwitchClusterList[] =
{
	{ZCL_CLUSTER_GEN_BASIC,			MANUFACTURER_CODE_NONE,	ZCL_BASIC_ATTR_NUM, 	basic_attrTbl,  	zcl_basic_register,		tuyaSwitch_basicCb},
	{ZCL_CLUSTER_GEN_IDENTIFY,		MANUFACTURER_CODE_NONE,	ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,	zcl_identify_register,	tuyaSwitch_identifyCb},
	{ZCL_CLUSTER_GEN_POWER_CFG,		MANUFACTURER_CODE_NONE,	ZCL_POWER_CFG_ATTR_NUM,	powerCfg_attrTbl,	zcl_powerCfg_register,	tuyaSwitch_powerCfgCb},
#ifdef ZCL_GROUP
	{ZCL_CLUSTER_GEN_GROUPS,		MANUFACTURER_CODE_NONE,	0, 						NULL,  				zcl_group_register,		tuyaSwitch_groupCb},
#endif
#ifdef ZCL_SCENE
	{ZCL_CLUSTER_GEN_SCENES,		MANUFACTURER_CODE_NONE,	0,						NULL,				zcl_scene_register,		tuyaSwitch_sceneCb},
#endif
#ifdef ZCL_POLL_CTRL
	{ZCL_CLUSTER_GEN_POLL_CONTROL,	MANUFACTURER_CODE_NONE,	ZCL_POLLCTRL_ATTR_NUM,	pollCtrl_attrTbl, 	zcl_pollCtrl_register, 	tuyaSwitch_pollCtrlCb},
#endif
};

u8 TUYA_SWITCH_CB_CLUSTER_NUM = (sizeof(g_tuyaSwitchClusterList)/sizeof(g_tuyaSwitchClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */




#endif	/* __PROJECT_TL_SWITCH__ */
