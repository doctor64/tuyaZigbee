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

#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "contactSensor.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


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
 * LOCAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb = 
{
	zbdemo_bdbInitCb, 
	zbdemo_bdbCommissioningCb, 
	zbdemo_bdbIdentifyCb, 
	NULL
};

#ifdef ZCL_OTA
ota_callBack_t contactSensor_otaCb =
{
	contactSensor_otaProcessMsgHandler,
};
#endif


/**********************************************************************
 * FUNCTIONS
 */
s32 contactSensor_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	return -1;
}

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
	printf("zbdemo_bdbCInitCb, status = %d, joinedNetwork = %d\n", status, joinedNetwork);	
	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * for non-factory-new device:
		 * 		load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
		 *
		 * for factory-new device:
		 * 		steer a network
		 *
		 */
		if(joinedNetwork){
			printf("zbdemo_bdbInitCb, set pollRate %d\n", POLL_RATE*MY_POLL_RATE_COEFF);
			zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);

#ifdef ZCL_OTA
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
#endif

#ifdef ZCL_POLL_CTRL
			contactSensor_zclCheckInStart();
#endif
		}else{
			u16 jitter = 0;
			do{
				jitter = zb_random() % 0x0fff;
			}while(jitter == 0);
			TL_ZB_TIMER_SCHEDULE(contactSensor_bdbNetworkSteerStart, NULL, jitter);
		}
	}else{
		if(joinedNetwork){
			zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
		}
	}
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
	printf("zbdemo_bdbCommissioningCb, status = %d\n", status);
	switch(status){
		case BDB_COMMISSION_STA_SUCCESS:
			light_blink_start(2, 200, 200);

			printf("zbdemo_bdbCommissioningCb, set pollRate %d\n", POLL_RATE*MY_POLL_RATE_COEFF);
			zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);

#ifdef ZCL_POLL_CTRL
			contactSensor_zclCheckInStart();
#endif
#ifdef ZCL_OTA
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
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
					jitter = zb_random() % 0x0fff;
				}while(jitter == 0);
				TL_ZB_TIMER_SCHEDULE(contactSensor_bdbNetworkSteerStart, NULL, jitter);
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
		case BDB_COMMISSION_STA_PARENT_LOST:
			//zb_rejoinSecModeSet(REJOIN_INSECURITY);
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			break;
		case BDB_COMMISSION_STA_REJOIN_FAILURE:
			break;
		default:
			break;
	}
}


extern void contactSensor_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
	contactSensor_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
}



#ifdef ZCL_OTA
void contactSensor_otaProcessMsgHandler(u8 evt, u8 status)
{
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){
			printf("contactSensor_otaProcessMsgHandler OTA_EVT_START, set pollRate %d\n", QUEUE_POLL_RATE);
			zb_setPollRate(QUEUE_POLL_RATE);
		}else{

		}
	}else if(evt == OTA_EVT_COMPLETE){
		printf("contactSensor_otaProcessMsgHandler OTA_EVT_COMPLETE, set pollRate %d\n", POLL_RATE*MY_POLL_RATE_COEFF);
		zb_setPollRate(POLL_RATE*MY_POLL_RATE_COEFF);

		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL*MY_OTA_QUERY_RATE_COEFF);
		}
	}
}
#endif

/*********************************************************************
 * @fn      contactSensor_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void contactSensor_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
	printf("contactSensor_leaveCnfHandler, status = %d\n", pLeaveCnf->status);
    if(pLeaveCnf->status == SUCCESS){
    	//SYSTEM_RESET();
    }
}

/*********************************************************************
 * @fn      contactSensor_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void contactSensor_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    printf("contactSensor_leaveIndHandler, rejoin = %d\n", pLeaveInd->rejoin);
    //printfArray(pLeaveInd->device_address, 8);
}


#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
