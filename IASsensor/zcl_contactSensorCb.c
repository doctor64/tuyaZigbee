/********************************************************************************************************
 * @file    zcl_contactSensorCb.c
 *
 * @brief   This is the source file for zcl_contactSensorCb
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
#include "contactSensor.h"
#include "app_ui.h"
#include "os/ev_timer.h"
#include "zcl_sensorControl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void contactSensor_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void contactSensor_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void contactSensor_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
#endif
#ifdef ZCL_REPORT
static void contactSensor_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void contactSensor_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void contactSensor_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd);
#endif
static void Sensor_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd);

/**********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      contactSensor_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void contactSensor_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	u16 cluster = pInHdlrMsg->msg->indInfo.cluster_id;
	switch(pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ_RSP:
			contactSensor_zclReadRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE_RSP:
			contactSensor_zclWriteRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE:
			contactSensor_zclWriteReqCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			contactSensor_zclCfgReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			contactSensor_zclCfgReportRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			contactSensor_zclReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			Sensor_zclDfltRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      contactSensor_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd)
{
    printf("contactSensor_zclReadRspCmd\n");

}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      contactSensor_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd)
{
    printf("contactSensor_zclWriteRspCmd\n");

}

/*********************************************************************
 * @fn      contactSensor_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
#ifdef ZCL_POLL_CTRL
	u8 numAttr = pWriteReqCmd->numAttr;
	zclWriteRec_t *attr = pWriteReqCmd->attrList;

	if(clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL){
		for(s32 i = 0; i < numAttr; i++){
			if(attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL){
				contactSensor_zclCheckInStart();
				return;
			}
			if(attr[i].attrID == ZCL_ATTRID_SC_LIGHT_SENSOR_RATE){
				contactSensor_zclLightSensorRate();
				return;
			}

		}
	}
#endif
}
#endif	/* ZCL_WRITE */


/*********************************************************************
 * @fn      contactSensor_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void Sensor_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd)
{
    printf("contactSensor_zclDfltRspCmd\n");

}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      contactSensor_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{
    printf("contactSensor_zclCfgReportCmd\n");

}

/*********************************************************************
 * @fn      contactSensor_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    printf("contactSensor_zclCfgReportRspCmd\n");

}

/*********************************************************************
 * @fn      contactSensor_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void contactSensor_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd)
{
    printf("contactSensor_zclReportCmd\n");

}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      contactSensor_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
s32 contactSensor_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void contactSensor_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      contactSensor_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param	endpoint
 * @param	srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void contactSensor_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		contactSensor_zclIdentifyTimerStop();
		light_blink_stop();
	}else{
		if(!identifyTimerEvt){
			light_blink_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(contactSensor_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      contactSensor_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void contactSensor_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
	//u8 effectVariant = pTriggerEffect->effectVariant;

	switch(effectId){
		case IDENTIFY_EFFECT_BLINK:
			light_blink_start(1, 500, 500);
			break;
		case IDENTIFY_EFFECT_BREATHE:
			light_blink_start(15, 300, 700);
			break;
		case IDENTIFY_EFFECT_OKAY:
			light_blink_start(2, 250, 250);
			break;
		case IDENTIFY_EFFECT_CHANNEL_CHANGE:
			light_blink_start(1, 500, 7500);
			break;
		case IDENTIFY_EFFECT_FINISH_EFFECT:
			light_blink_start(1, 300, 700);
			break;
		case IDENTIFY_EFFECT_STOP_EFFECT:
			light_blink_stop();
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      contactSensor_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void contactSensor_zclIdentifyQueryRspCmdHandler(u8 endpoint, u16 srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
#if FIND_AND_BIND_SUPPORT
	if(identifyRsp->timeout){
		findBindDst_t dstInfo;
		dstInfo.addr = srcAddr;
		dstInfo.endpoint = endpoint;

		bdb_addIdentifyActiveEpForFB(dstInfo);
	}
#endif
}

/*********************************************************************
 * @fn      contactSensor_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == CONTACT_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_IDENTIFY:
					contactSensor_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
					break;
				case ZCL_CMD_TRIGGER_EFFECT:
					contactSensor_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
					break;
				default:
					break;
			}
		}else{
			if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
				contactSensor_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IDENTIFY */

#ifdef ZCL_IAS_ZONE
/*********************************************************************
 * @fn      contactSensor_zclIasZoneEnrollRspCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE Enroll response command.
 *
 * @param   pZoneEnrollRsp
 *
 * @return  None
 */
static void contactSensor_zclIasZoneEnrollRspCmdHandler(zoneEnrollRsp_t *pZoneEnrollRsp)
{
	printf("zclIasZoneEnrollRspCmdHandler code:%d zone_id:%d\n", pZoneEnrollRsp->code, pZoneEnrollRsp->zoneId);
	if (pZoneEnrollRsp->zoneId != ZCL_ZONE_ID_INVALID) {
		u8 zoneState;
		zoneState = ZONE_STATE_ENROLLED;
		zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &(pZoneEnrollRsp->zoneId));
		zcl_setAttrVal(CONTACT_SENSOR_ENDPOINT, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &zoneState);
	}
}

/*********************************************************************
 * @fn      contactSensor_zclIasZoneInitNormalOperationModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE normal operation mode command.
 *
 * @param
 *
 * @return  status
 */
static status_t contactSensor_zclIasZoneInitNormalOperationModeCmdHandler(void)
{
	printf("zclIasZoneInitNormalOperationModeCmdHandler\n");
	u8 status = ZCL_STA_FAILURE;

	return status;
}

/*********************************************************************
 * @fn      contactSensor_zclIasZoneInitTestModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE test mode command.
 *
 * @param   pZoneInitTestMode
 *
 * @return  status
 */
static status_t contactSensor_zclIasZoneInitTestModeCmdHandler(zoneInitTestMode_t *pZoneInitTestMode)
{
	printf("zclIasZoneInitNormalOperationModeCmdHandler\n");
	u8 status = ZCL_STA_FAILURE;

	return status;
}

/*********************************************************************
 * @fn      contactSensor_iasZoneCb
 *
 * @brief   Handler for ZCL IAS Zone command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == CONTACT_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_ZONE_ENROLL_RSP:
					contactSensor_zclIasZoneEnrollRspCmdHandler((zoneEnrollRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_INIT_NORMAL_OPERATION_MODE:
					contactSensor_zclIasZoneInitNormalOperationModeCmdHandler();
					break;
				case ZCL_CMD_INIT_TEST_MODE:
					contactSensor_zclIasZoneInitTestModeCmdHandler((zoneInitTestMode_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif  /* ZCL_IAS_ZONE */

/*********************************************************************
 * @fn      contactSensor_powerCfgCb
 *
 * @brief   Handler for ZCL PowerCfg command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
//	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
//	}

	return ZCL_STA_SUCCESS;
}


#ifdef ZCL_POLL_CTRL
static ev_timer_event_t *zclFastPollTimeoutTimerEvt = NULL;
static ev_timer_event_t *zclCheckInTimerEvt = NULL;
static bool isFastPollMode = FALSE;

void contactSensor_zclCheckInCmdSend(void)
{
	printf("contactSensor_zclCheckInCmdSend()\n");
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
	dstEpInfo.dstEp = CONTACT_SENSOR_ENDPOINT;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_pollCtrl_checkInCmd(CONTACT_SENSOR_ENDPOINT, &dstEpInfo, TRUE);
}

s32 contactSensor_zclCheckInTimerCb(void *arg)
{
	printf("contactSensor_zclCheckInTimerCb()\n");
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();
	printf("zclCheckInTimerCb time %d\n", pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);

	if(!pPollCtrlAttr->chkInInterval){
		zclCheckInTimerEvt = NULL;
		return -1;
	}

	contactSensor_zclCheckInCmdSend();

	return 0;
}

void contactSensor_zclCheckInStart(void)
{
	printf("zclCheckInStart1\n");
	if(zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, CONTACT_SENSOR_ENDPOINT)){
		zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

		// if checkin timer already running, stop it
		if (zclCheckInTimerEvt){
			printf("Cancel timer\n");
			TL_ZB_TIMER_CANCEL(&zclCheckInTimerEvt);
		}


		if(!zclCheckInTimerEvt){
			printf("zclCheckInStart2 time 0x%x %x 1/4 s\n", (u16)((pPollCtrlAttr->chkInInterval&0xff00)>>16), (u16)(pPollCtrlAttr->chkInInterval&0x00ff));
			zclCheckInTimerEvt = TL_ZB_TIMER_SCHEDULE(contactSensor_zclCheckInTimerCb, NULL, pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);

			if(pPollCtrlAttr->chkInInterval){
				contactSensor_zclCheckInCmdSend();
			}
		}
	}
}

void contactSensor_zclSetFastPollMode(bool fastPollMode)
{
	printf("contactSensor_zclSetFastPollMode(), fastPollMode %d\n", fastPollMode);
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	isFastPollMode = fastPollMode;
	u32 pollRate = fastPollMode ? pPollCtrlAttr->shortPollInterval : pPollCtrlAttr->longPollInterval;

	printf("contactSensor_zclSetFastPollMode(), pollRate %d\n", pollRate * POLL_RATE_QUARTERSECONDS);
	zb_setPollRate(pollRate * POLL_RATE_QUARTERSECONDS);
}

s32 contactSensor_zclFastPollTimeoutCb(void *arg)
{
	contactSensor_zclSetFastPollMode(FALSE);

	zclFastPollTimeoutTimerEvt = NULL;
	return -1;
}

static status_t contactSensor_zclPollCtrlChkInRspCmdHandler(zcl_chkInRsp_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(pCmd->startFastPolling){
		u32 fastPollTimeoutCnt = 0;

		if(pCmd->fastPollTimeout){
			if(pCmd->fastPollTimeout > pPollCtrlAttr->fastPollTimeoutMax){
				return ZCL_STA_INVALID_FIELD;
			}

			fastPollTimeoutCnt = pCmd->fastPollTimeout;

			if(zclFastPollTimeoutTimerEvt){
				TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
			}
		}else{
			if(!zclFastPollTimeoutTimerEvt){
				fastPollTimeoutCnt = pPollCtrlAttr->fastPollTimeout;
			}
		}

		if(!zclFastPollTimeoutTimerEvt && fastPollTimeoutCnt){
			contactSensor_zclSetFastPollMode(TRUE);

			zclFastPollTimeoutTimerEvt = TL_ZB_TIMER_SCHEDULE(contactSensor_zclFastPollTimeoutCb, NULL, fastPollTimeoutCnt * POLL_RATE_QUARTERSECONDS);
		}
	}else{
		//continue in normal operation and not required to go into fast poll mode.
	}

	return ZCL_STA_SUCCESS;
}

static status_t contactSensor_zclPollCtrlFastPollStopCmdHandler(void)
{
	if(!isFastPollMode){
		return ZCL_STA_ACTION_DENIED;
	}else{
		if(zclFastPollTimeoutTimerEvt){
			TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
		}
		contactSensor_zclSetFastPollMode(FALSE);
	}

	return ZCL_STA_SUCCESS;
}

static status_t contactSensor_zclPollCtrlSetLongPollIntervalCmdHandler(zcl_setLongPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newLongPollInterval >= 0x04) && (pCmd->newLongPollInterval <= 0x6E0000)
		&& (pCmd->newLongPollInterval <= pPollCtrlAttr->chkInInterval) && (pCmd->newLongPollInterval >= pPollCtrlAttr->shortPollInterval)){
		pPollCtrlAttr->longPollInterval = pCmd->newLongPollInterval;
	    printf("contactSensor_zclPollCtrlSetLongPollIntervalCmdHandler, set pollRate %d\n", pCmd->newLongPollInterval * POLL_RATE_QUARTERSECONDS);
		zb_setPollRate(pCmd->newLongPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

static status_t contactSensor_zclPollCtrlSetShortPollIntervalCmdHandler(zcl_setShortPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newShortPollInterval >= 0x01) && (pCmd->newShortPollInterval <= 0xff)
		&& (pCmd->newShortPollInterval <= pPollCtrlAttr->longPollInterval)){
		pPollCtrlAttr->shortPollInterval = pCmd->newShortPollInterval;
		printf("contactSensor_zclPollCtrlSetShortPollIntervalCmdHandler, set pollRate %d\n", pCmd->newShortPollInterval * POLL_RATE_QUARTERSECONDS);
		zb_setPollRate(pCmd->newShortPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      contactSensor_pollCtrlCb
 *
 * @brief   Handler for ZCL Poll Control command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == CONTACT_SENSOR_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_CHK_IN_RSP:
					status = contactSensor_zclPollCtrlChkInRspCmdHandler((zcl_chkInRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_FAST_POLL_STOP:
					status = contactSensor_zclPollCtrlFastPollStopCmdHandler();
					break;
				case ZCL_CMD_SET_LONG_POLL_INTERVAL:
					status = contactSensor_zclPollCtrlSetLongPollIntervalCmdHandler((zcl_setLongPollInterval_t *)cmdPayload);
					break;
				case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
					status = contactSensor_zclPollCtrlSetShortPollIntervalCmdHandler((zcl_setShortPollInterval_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif	/* ZCL_POLL_CTRL */

#ifdef HAVE_LIGHT_SENSOR
/*********************************************************************
 * @fn      contactSensor_illuminanceCb
 *
 * @brief   Handler for ZCL Illuminance command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t contactSensor_illuminanceCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
//	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
//	}

	return ZCL_STA_SUCCESS;
}
#endif //HAVE_LIGHT_SENSOR


void contactSensor_zclLightSensorRate(void)
{
	printf("contactSensor_zclLightSensorRate\n");
	//if(zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, CONTACT_SENSOR_ENDPOINT)){
		zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

		// if checkin timer already running, stop it
		if (g_sensorAppCtx.timerLightLevelEvt){
			printf("Cancel light sensor timer\n");
			TL_ZB_TIMER_CANCEL(&g_sensorAppCtx.timerLightLevelEvt);
		}


		if(!g_sensorAppCtx.timerLightLevelEvt){
			printf("contactSensor_zclLightSensorRate time 0x%x %x 1/4 s\n", (u16)((pPollCtrlAttr->chkLightInterval &0xffff0000)>>16), (u16)(pPollCtrlAttr->chkLightInterval&0xffff));
			g_sensorAppCtx.timerLightLevelEvt = TL_ZB_TIMER_SCHEDULE(lightLevelCb, NULL, pPollCtrlAttr->chkLightInterval);

		}
//	}
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
