/********************************************************************************************************
 * @file    contactSensor.c
 *
 * @brief   This is the source file for contactSensor
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
#include "../common/comm_cfg.h"
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "contactSensor.h"
#include "app_ui.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
app_ctx_t g_sensorAppCtx;


#ifdef ZCL_OTA
extern ota_callBack_t contactSensor_otaCb;

//running code firmware information
ota_preamble_t contactSensor_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	contactSensor_leaveIndHandler,//leave ind cb
	contactSensor_leaveCnfHandler,//leave cnf cb
	NULL,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
	.touchlinkEnable = 1,												/* enable touch-link */
#else
	.touchlinkEnable = 0,												/* disable touch-link */
#endif
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
drv_pm_pinCfg_t g_sensorPmCfg[] = {
	{
		BUTTON1,
		PM_WAKEUP_LEVEL_BUTTON1
	},
	{
		BUTTON2,
		PM_WAKEUP_LEVEL_BUTTON2
	}
};
#endif
/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	printf("stack_init\n");
	zb_init();
	zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	printf("user_app_init\n");
#if ZCL_POLL_CTRL_SUPPORT
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_PERIODICALLY);
#else
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
#endif

	/* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(contactSensor_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(CONTACT_SENSOR_ENDPOINT, (af_simple_descriptor_t *)&contactSensor_simpleDesc, zcl_rx_handler, NULL);

	zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	zcl_register(CONTACT_SENSOR_ENDPOINT, CONTACT_SENSOR_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_contactSensorClusterList);

#if ZCL_OTA_SUPPORT
	ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&contactSensor_simpleDesc, &contactSensor_otaInfo, &contactSensor_otaCb);
#endif

	// battery monitor
	//g_sensorAppCtx.timerBattEvt = TL_ZB_TIMER_SCHEDULE(battVoltageCb, NULL, 500);//BATTERY_CHECK_INTERVAL);
	g_sensorAppCtx.timerLightLevelEvt = NULL;
	contactSensor_zclLightSensorRate();
	//TL_ZB_TIMER_SCHEDULE(lightLevelCb, NULL, 20000);
}



void led_init(void)
{
	light_init();
}

void report_handler(void)
{
	if(zb_isDeviceJoinedNwk()){
		if(zcl_reportingEntryActiveNumGet()){
			u16 second = 1;//TODO: fix me

			reportNoMinLimit();

			//start report timer
			reportAttrTimerStart(second);
		}else{
			//stop report timer
			reportAttrTimerStop();
		}
	}
}

void app_task(void)
{
	app_key_handler();

	if(bdb_isIdle()){
#if PM_ENABLE
		if(!g_sensorAppCtx.keyPressed){
			//printf("Sleep\n");
			drv_pm_lowPowerEnter();
		}
#endif

		report_handler();
	}
}

static void contactSensorSysException(void)
{
#if 1
	SYSTEM_RESET();
#else
	light_on();
	while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
//	printf("user init(%d)\n", isRetention);
	/* Initialize LEDs*/
	led_init();

#if PA_ENABLE
	rf_paInit(PA_TX, PA_RX);
#endif

#if ZBHCI_EN
	zbhciInit();
#endif

#if PM_ENABLE
	drv_pm_wakeupPinConfig(g_sensorPmCfg, sizeof(g_sensorPmCfg)/sizeof(drv_pm_pinCfg_t));
#endif

	if(!isRetention){
		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		/* Register except handler for test */
		sys_exceptHandlerRegister(contactSensorSysException);

		/* User's Task */
#if ZBHCI_EN
		ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* Load the pre-install code from flash */
		if(bdb_preInstallCodeLoad(&g_sensorAppCtx.tcLinkKey.keyType, g_sensorAppCtx.tcLinkKey.key) == RET_OK){
			g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_sensorAppCtx.tcLinkKey.keyType;
			g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_sensorAppCtx.tcLinkKey.key;
		}

		/* Set default reporting configuration */
		u16 reportableChange = ZCL_REPORTING_DEFAULT_REP_CHANGE;
		bdb_defaultReportingCfg(CONTACT_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE,
								ZCL_REPORTING_DEFAULT_MIN_INTERVAL, ZCL_REPORTING_DEFAULT_MAX_INTERVAL, (u8 *)&reportableChange);
		bdb_defaultReportingCfg(CONTACT_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,
								ZCL_REPORTING_DEFAULT_MIN_INTERVAL, ZCL_REPORTING_DEFAULT_MAX_INTERVAL, (u8 *)&reportableChange);
		reportableChange = 500;
		bdb_defaultReportingCfg(CONTACT_SENSOR_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT, ZCL_ATTRID_MEASURED_VALUE,
								ZCL_REPORTING_DEFAULT_MIN_INTERVAL, ZCL_REPORTING_DEFAULT_MAX_INTERVAL, (u8 *)&reportableChange);


		/* Initialize BDB */
		u8 repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
		bdb_init((af_simple_descriptor_t *)&contactSensor_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, repower);
	}else{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
