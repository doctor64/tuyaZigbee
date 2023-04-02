/********************************************************************************************************
 * @file    zb_appCb.c
 *
 * @brief   This is the source file for zb_appCb
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
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "tuyaLight.h"
#include "tuyaLightCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define DEBUG_HEART		0

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork);
void zbdemo_bdbCommissioningCb(u8 status, void *arg);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);


/**********************************************************************
 * GLOBAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb = {zbdemo_bdbInitCb, zbdemo_bdbCommissioningCb, zbdemo_bdbIdentifyCb, NULL};

#ifdef ZCL_OTA
ota_callBack_t tuyaLight_otaCb =
{
	tuyaLight_otaProcessMsgHandler,
};
#endif

/**********************************************************************
 * LOCAL VARIABLES
 */
u32 heartInterval = 0;

#if DEBUG_HEART
ev_timer_event_t *heartTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */
#if DEBUG_HEART
static s32 heartTimerCb(void *arg){
	if(heartInterval == 0){
		heartTimerEvt = NULL;
		return -1;
	}

	gpio_toggle(LED_POWER);

	return heartInterval;
}
#endif

s32 tuyaLight_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	return -1;
}

#if FIND_AND_BIND_SUPPORT
s32 tuyaLight_bdbFindAndBindStart(void *arg){
	bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_TARGET);

	return -1;
}
#endif

/*********************************************************************
 * @fn      zbdemo_bdbInitCb
 *
 * @brief   application callback for bdb initiation
 *
 * @param   status - the status of bdb init BDB_INIT_STATUS_SUCCESS or BDB_INIT_STATUS_FAILURE
 *
 * @param   joinedNetwork  - 1: node is on a network, 0: node isn't on a network
 *
 * @return  None
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork){
//	printf("bdbInitCb: sta = %x, joined = %x\n", status, joinedNetwork);

	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * start bdb commissioning
		 * */
		if(joinedNetwork){
			heartInterval = 1000;

#ifdef ZCL_OTA
			ota_queryStart(6*60*60);//OTA_PERIODIC_QUERY_INTERVAL);
#endif
		}else{
			heartInterval = 500;

#if	(!ZBHCI_EN)
			u16 jitter = 0;
			do{
				jitter = zb_random() % 0x0fff;
			}while(jitter == 0);
			TL_ZB_TIMER_SCHEDULE(tuyaLight_bdbNetworkSteerStart, NULL, jitter);
#endif
		}
	}else{
		heartInterval = 200;
	}

#if DEBUG_HEART
	if(heartTimerEvt){
		TL_ZB_TIMER_CANCEL(&heartTimerEvt);
	}
	heartTimerEvt = TL_ZB_TIMER_SCHEDULE(heartTimerCb, NULL, heartInterval);
#endif
}

/*********************************************************************
 * @fn      zbdemo_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void zbdemo_bdbCommissioningCb(u8 status, void *arg){
//	printf("bdbCommCb: sta = %x\n", status);

	switch(status){
		case BDB_COMMISSION_STA_SUCCESS:
			heartInterval = 1000;

			light_blink_start(2, 200, 200);

#ifdef ZCL_OTA
	    	ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
#endif

#if FIND_AND_BIND_SUPPORT
			if(!gLightCtx.bdbFindBindFlg){
				gLightCtx.bdbFindBindFlg = TRUE;
				TL_ZB_TIMER_SCHEDULE(tuyaLight_bdbFindAndBindStart, NULL, 1000);
			}
#endif
			break;
		case BDB_COMMISSION_STA_IN_PROGRESS:
			break;
		case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
			break;
		case BDB_COMMISSION_STA_NO_NETWORK:
		case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
		case BDB_COMMISSION_STA_TARGET_FAILURE:
			{
				u16 jitter = 0;
				do{
					jitter = zb_random() % 0x2710;
				}while(jitter < 5000);
				TL_ZB_TIMER_SCHEDULE(tuyaLight_bdbNetworkSteerStart, NULL, jitter);
			}
			break;
		case BDB_COMMISSION_STA_FORMATION_FAILURE:
			break;
		case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
			break;
		case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
			break;
		case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
			break;
		case BDB_COMMISSION_STA_NOT_PERMITTED:
			break;
		case BDB_COMMISSION_STA_REJOIN_FAILURE:
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			break;
		case BDB_COMMISSION_STA_FORMATION_DONE:
#ifndef ZBHCI_EN
			tl_zbMacChannelSet(DEFAULT_CHANNEL);  //set default channel
#endif
			break;
		default:
			break;
	}
}


extern void tuyaLight_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
#if FIND_AND_BIND_SUPPORT
	tuyaLight_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
#endif
}



#ifdef ZCL_OTA
void tuyaLight_otaProcessMsgHandler(u8 evt, u8 status)
{
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){

		}else{

		}
	}else if(evt == OTA_EVT_COMPLETE){
		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
		}
	}
}
#endif

s32 tuyaLight_softReset(void *arg){
	SYSTEM_RESET();

	return -1;
}

/*********************************************************************
 * @fn      tuyaLight_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void tuyaLight_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
    	light_blink_start(3, 200, 200);

    	//waiting blink over
    	TL_ZB_TIMER_SCHEDULE(tuyaLight_softReset, NULL, 2 * 1000);
    }
}

/*********************************************************************
 * @fn      tuyaLight_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void tuyaLight_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{

}

bool tuyaLight_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate){
	return FAILURE;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
