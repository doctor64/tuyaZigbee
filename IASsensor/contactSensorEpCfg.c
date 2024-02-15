/********************************************************************************************************
 * @file    contactSensorEpCfg.c
 *
 * @brief   This is the source file for contactSensorEpCfg
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

#include "app_cfg.h"
#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "types.h"
#include "zcl_const.h"
#include "tl_common.h"
#include "zcl_include.h"
#include "zcl_onoffSwitchCfg.h"
//#include "security_safety/zcl_ias_zone.h"
#include "contactSensor.h"
#include "zcl_sensorControl.h"
#include "timestamp.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   {8,'T','L','S','R','8','2','x','x'}
#endif


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 contactSensor_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_IAS_ZONE
	ZCL_CLUSTER_SS_IAS_ZONE,
#endif
#ifdef ZCL_POLL_CTRL
	ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
#ifdef ZCL_ON_OFF_SWITCH_CFG
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 contactSensor_outClusterList[] =
{
#ifdef ZCL_ON_OFF
ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_OTA
ZCL_CLUSTER_OTA,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define CONTACTSENSOR_IN_CLUSTER_NUM		(sizeof(contactSensor_inClusterList)/sizeof(contactSensor_inClusterList[0]))
#define CONTACTSENSOR_OUT_CLUSTER_NUM	(sizeof(contactSensor_outClusterList)/sizeof(contactSensor_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t contactSensor_simpleDesc =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_IAS_ZONE,                	    /* Application device identifier */
	CONTACT_SENSOR_ENDPOINT,         		/* Endpoint */
	0,                                  	/* Application device version */
	0,										/* Reserved */
	CONTACTSENSOR_IN_CLUSTER_NUM,           	/* Application input cluster count */
	CONTACTSENSOR_OUT_CLUSTER_NUM,          	/* Application output cluster count */
	(u16 *)contactSensor_inClusterList,    	/* Application input cluster list */
	(u16 *)contactSensor_outClusterList,   	/* Application output cluster list */
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
	.dateCode       = ZCL_BASIC_DATE_CODE,
	.powerSource	= POWER_SOURCE_BATTERY,
	.deviceEnable	= TRUE,
	.swBuildId		= ZCL_BASIC_SW_BUILD_ID,
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
	{ ZCL_ATTRID_BASIC_DEV_ENABLED,  		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},
	{ ZCL_ATTRID_BASIC_SW_BUILD_ID,  		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)&git_short_hash_str}, //g_zcl_basicAttrs.swBuildId},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_BASIC_ATTR_NUM 				sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


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

#define ZCL_IDENTIFY_ATTR_NUM			sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

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

#ifdef ZCL_IAS_ZONE
/* IAS Zone */
zcl_iasZoneAttr_t g_zcl_iasZoneAttrs =
{
	.zoneState		= ZONE_STATE_NOT_ENROLLED,
	.zoneType		= ZONE_TYPE_CONTACT_SWITCH,
	.zoneStatus		= ZONE_STATUS_BIT_RESTORE_NOTIFY, //device will automaticaly send reports about alarm cleared
	.iasCieAddr		= {0x00},
	.zoneId 		= ZCL_ZONE_ID_INVALID,
};

const zclAttrInfo_t iasZone_attrTbl[] =
{
	{ ZCL_ATTRID_ZONE_STATE,   ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ,  						(u8*)&g_zcl_iasZoneAttrs.zoneState },
	{ ZCL_ATTRID_ZONE_TYPE,    ZCL_DATA_TYPE_ENUM16,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_iasZoneAttrs.zoneType },
	{ ZCL_ATTRID_ZONE_STATUS,  ZCL_DATA_TYPE_BITMAP16,  ACCESS_CONTROL_READ,  						(u8*)&g_zcl_iasZoneAttrs.zoneStatus },
	{ ZCL_ATTRID_IAS_CIE_ADDR, ZCL_DATA_TYPE_IEEE_ADDR, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)g_zcl_iasZoneAttrs.iasCieAddr },
	{ ZCL_ATTRID_ZONE_ID,	   ZCL_DATA_TYPE_UINT8,     ACCESS_CONTROL_READ,  						(u8*)&g_zcl_iasZoneAttrs.zoneId},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  				(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_IASZONE_ATTR_NUM		 sizeof(iasZone_attrTbl) / sizeof(zclAttrInfo_t)
#endif //ZCL_IAS_ZONE

#ifdef ZCL_POLL_CTRL
/* Poll Control */
zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs =
{
	.chkInInterval			= 0x3840,
	.longPollInterval		= 0x14,
	.shortPollInterval		= 0x02,
	.fastPollTimeout		= 0x28,
	.chkInIntervalMin		= 0x00,
	.longPollIntervalMin	= 0x00,
	.fastPollTimeoutMax		= 0x00,

	.chkBattInterval        = 0x00,
	.chkLightInterval       = BATTERY_CHECK_INTERVAL,
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

    { ZCL_ATTRID_SC_LIGHT_SENSOR_RATE,  ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.chkLightInterval},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  					  (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_POLLCTRL_ATTR_NUM		 sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif //ZCL_POLL_CTRL

#ifdef HAVE_LIGHT_SENSOR
/* illuminance measurement */
zcl_illuminanceAttr_t g_zcl_illuminanceMSAttrs =
{
	.measuredValue 		= 0x0,
	.minMeasuredValue 	= 0x1,
	.maxMeasuredValue 	= 0xfffe,
	.tolerance 			= 0x1,
	.lightSensorType    = 0x40, // not defined in ZCL - photoresistor
};

const zclAttrInfo_t illuminance_ms_attrTbl[] =
{
	{ ZCL_ATTRID_MEASURED_VALUE,      ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_zcl_illuminanceMSAttrs.measuredValue },
	{ ZCL_ATTRID_MIN_MEASURED_VALUE,  ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,  (u8*)&g_zcl_illuminanceMSAttrs.minMeasuredValue },
	{ ZCL_ATTRID_MAX_MEASURED_VALUE,  ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,  (u8*)&g_zcl_illuminanceMSAttrs.maxMeasuredValue },
	{ ZCL_ATTRID_TOLERANCE,           ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,  (u8*)&g_zcl_illuminanceMSAttrs.tolerance },
	{ ZCL_ATTRID_LIGHT_SENSOR_TYPE,   ZCL_DATA_TYPE_ENUM8,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_illuminanceMSAttrs.lightSensorType },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_ILLUMINANCE_MS_ATTR_NUM		 sizeof(illuminance_ms_attrTbl) / sizeof(zclAttrInfo_t)
#endif //HAVE_LIGHT_SENSOR

#ifdef ZCL_ON_OFF_SWITCH_CFG
/* On/Off Config */
zcl_onOffSwitchCfg g_zcl_onOffSwitchCfgAttrs =
{
	.switchType			= ZCL_SWITCH_TYPE_TOGGLE, //Toggle
	.switchActions	 	= ZCL_SWITCH_ACTION_TOGGLE, 
};

const zclAttrInfo_t on_off_switch_config_attrTbl[] =
{
	{ ZCL_ATTRID_SWITCH_TYPE,      ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ,  (u8*)&g_zcl_onOffSwitchCfgAttrs.switchType },
	{ ZCL_ATTRID_SWITCH_ACTION,  ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&g_zcl_onOffSwitchCfgAttrs.switchActions },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM		 sizeof(on_off_switch_config_attrTbl) / sizeof(zclAttrInfo_t)
#endif //ZCL_ON_OFF_SWITCH_CFG
/**
 *  @brief Definition for IAS sensor ZCL specific cluster
 */
const zcl_specClusterInfo_t g_contactSensorClusterList[] =
{
	{ZCL_CLUSTER_GEN_BASIC,			MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM, 	basic_attrTbl,  	zcl_basic_register,		contactSensor_basicCb},
	{ZCL_CLUSTER_GEN_IDENTIFY,		MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,	zcl_identify_register,	contactSensor_identifyCb},
	{ZCL_CLUSTER_GEN_POWER_CFG,		MANUFACTURER_CODE_NONE,	ZCL_POWER_CFG_ATTR_NUM,	powerCfg_attrTbl,	zcl_powerCfg_register,	contactSensor_powerCfgCb},
#ifdef ZCL_IAS_ZONE
	{ZCL_CLUSTER_SS_IAS_ZONE,		MANUFACTURER_CODE_NONE, ZCL_IASZONE_ATTR_NUM,	iasZone_attrTbl,	zcl_iasZone_register,	contactSensor_iasZoneCb},
#endif
#ifdef ZCL_POLL_CTRL
	{ZCL_CLUSTER_GEN_POLL_CONTROL,  MANUFACTURER_CODE_NONE, ZCL_POLLCTRL_ATTR_NUM, 	pollCtrl_attrTbl,   zcl_pollCtrl_register,	contactSensor_pollCtrlCb},
#endif
#ifdef HAVE_LIGHT_SENSOR
	{ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, MANUFACTURER_CODE_NONE, ZCL_ILLUMINANCE_MS_ATTR_NUM, illuminance_ms_attrTbl, zcl_illuminanceMeasure_register, contactSensor_illuminanceCb},
#endif	
#ifdef ZCL_ON_OFF_SWITCH_CFG
	{ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, MANUFACTURER_CODE_NONE, ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM, on_off_switch_config_attrTbl, zcl_onOffSwitchCfg_register, NULL},
#endif	
};

u8 CONTACT_SENSOR_CB_CLUSTER_NUM = (sizeof(g_contactSensorClusterList)/sizeof(g_contactSensorClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */




#endif	/* __PROJECT_TL_CONTACT_SENSOR__ */
