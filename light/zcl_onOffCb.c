/********************************************************************************************************
 * @file    zcl_onOffCb.c
 *
 * @brief   This is the source file for zcl_onOffCb
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
#include "tuyaLight.h"
#include "tuyaLightCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster

/**********************************************************************
 * LOCAL VARIABLES
 */
static ev_timer_event_t *onWithTimedOffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      tuyaLight_onOffInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void tuyaLight_onOffInit(void)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	tuyaLight_onoff(pOnOff->onOff);
}

/*********************************************************************
 * @fn      tuyaLight_updateOnOff
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void tuyaLight_updateOnOff(void)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	hwLight_onOffUpdate(pOnOff->onOff);
}

/*********************************************************************
 * @fn      tuyaLight_onoff
 *
 * @brief
 *
 * @param   ZCL_CMD_ONOFF_ON / ZCL_ONOFF_STATUS_OFF
 *
 * @return  None
 */
void tuyaLight_onoff(u8 cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(cmd == ZCL_CMD_ONOFF_ON){
		pOnOff->globalSceneControl = TRUE;

		pOnOff->onOff = ZCL_ONOFF_STATUS_ON;
		if(pOnOff->onTime == 0){
			pOnOff->offWaitTime = 0;
		}
	}else if(cmd == ZCL_CMD_ONOFF_OFF){
		pOnOff->onOff = ZCL_ONOFF_STATUS_OFF;
		pOnOff->onTime = 0;
	}else{
		if(pOnOff->onOff == ZCL_ONOFF_STATUS_OFF){
			pOnOff->globalSceneControl = TRUE;

			pOnOff->onOff = ZCL_ONOFF_STATUS_ON;
			if(pOnOff->onTime == 0){
				pOnOff->offWaitTime = 0;
			}
		}else{
			pOnOff->onOff = ZCL_ONOFF_STATUS_OFF;
			pOnOff->onTime = 0;
		}
	}

	light_fresh();

#ifdef ZCL_SCENE
	zcl_sceneAttr_t *pScene = zcl_sceneAttrGet();
	pScene->sceneValid = 0;
#endif
}

/*********************************************************************
 * @fn      tuyaLight_OnWithTimedOffTimerCb
 *
 * @brief   timer event to process the ON_WITH_TIMED_OFF command
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 tuyaLight_OnWithTimedOffTimerCb(void *arg)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_ON) && pOnOff->onTime){
		pOnOff->onTime--;
		if(pOnOff->onTime <= 0){
			pOnOff->offWaitTime = 0;
			tuyaLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_OFF) && pOnOff->offWaitTime){
		pOnOff->offWaitTime--;
		if(pOnOff->offWaitTime <= 0){
			onWithTimedOffTimerEvt = NULL;
			return -1;
		}
	}

	if(pOnOff->onTime || pOnOff->offWaitTime){
		return 0;
	}else{
		onWithTimedOffTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      tuyaLight_OnWithTimedOffTimerStop
 *
 * @brief   force to stop the OnWithTimedOff timer
 *
 * @param
 *
 * @return
 */
static void tuyaLight_OnWithTimedOffTimerStop(void)
{
	if(onWithTimedOffTimerEvt){
		TL_ZB_TIMER_CANCEL(&onWithTimedOffTimerEvt);
	}
}

/*********************************************************************
 * @fn      tuyaLight_onoff_onWithTimedOffProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void tuyaLight_onoff_onWithTimedOffProcess(zcl_onoff_onWithTimeOffCmd_t *cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(cmd->onOffCtrl.bits.acceptOnlyWhenOn && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		return;
	}

	if(pOnOff->offWaitTime && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		pOnOff->offWaitTime = min2(pOnOff->offWaitTime, cmd->offWaitTime);
	}else{
		pOnOff->onTime = max2(pOnOff->onTime, cmd->onTime);
		pOnOff->offWaitTime = cmd->offWaitTime;
		tuyaLight_onoff(ZCL_CMD_ONOFF_ON);
	}

	if((pOnOff->onTime < 0xFFFF) && (pOnOff->offWaitTime < 0xFFFF)){
		if(pOnOff->onTime || pOnOff->offWaitTime){
			tuyaLight_OnWithTimedOffTimerStop();
			onWithTimedOffTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaLight_OnWithTimedOffTimerCb, NULL, ZCL_ONOFF_TIMER_INTERVAL);
		}
	}
}

/*********************************************************************
 * @fn      tuyaLight_onoff_offWithEffectProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void tuyaLight_onoff_offWithEffectProcess(zcl_onoff_offWithEffectCmd_t *cmd)
{
	//TODO: FIXED ME

	tuyaLight_onoff(ZCL_CMD_ONOFF_OFF);
}

/*********************************************************************
 * @fn      tuyaLight_onoff_onWithRecallGlobalSceneProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void tuyaLight_onoff_onWithRecallGlobalSceneProcess(void)
{

}

/*********************************************************************
 * @fn      tuyaLight_onOffCb
 *
 * @brief   Handler for ZCL ONOFF command. This function will set ONOFF attribute first.
 *
 * @param	pAddrInfo
 * @param   cmdId - onoff cluster command id
 * @param	cmdPayload
 *
 * @return  status_t
 */
status_t tuyaLight_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(pAddrInfo->dstEp == TUYA_LIGHT_ENDPOINT){
		switch(cmdId){
			case ZCL_CMD_ONOFF_ON:
			case ZCL_CMD_ONOFF_OFF:
			case ZCL_CMD_ONOFF_TOGGLE:
				tuyaLight_onoff(cmdId);
				break;
			case ZCL_CMD_OFF_WITH_EFFECT:
				if(pOnOff->globalSceneControl == TRUE){
					/* TODO: store its settings in its global scene */

					pOnOff->globalSceneControl = FALSE;
				}
				tuyaLight_onoff_offWithEffectProcess((zcl_onoff_offWithEffectCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:
				if(pOnOff->globalSceneControl == FALSE){
					tuyaLight_onoff_onWithRecallGlobalSceneProcess();
					pOnOff->globalSceneControl = TRUE;
				}
				break;
			case ZCL_CMD_ON_WITH_TIMED_OFF:
				tuyaLight_onoff_onWithTimedOffProcess((zcl_onoff_onWithTimeOffCmd_t *)cmdPayload);
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
