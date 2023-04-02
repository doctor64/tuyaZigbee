/********************************************************************************************************
 * @file    zcl_sceneCb.c
 *
 * @brief   This is the source file for zcl_sceneCb
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


/*********************************************************************
 * @fn      tuyaLight_sceneRecallReqHandler
 *
 * @brief   Handler for ZCL scene recall command. This function will recall scene.
 *
 * @param	pApsdeInd
 * @param	pScene
 *
 * @return  None
 */
static void tuyaLight_sceneRecallReqHandler(zclIncomingAddrInfo_t *pAddrInfo, zcl_sceneEntry_t *pScene)
{
	u8 extLen = 0;

#ifdef ZCL_ON_OFF
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	pOnOff->onOff = pScene->extField[extLen+3];
	extLen += 4;
#endif

#ifdef ZCL_LEVEL_CTRL
	u8 level = pScene->extField[extLen+3];
	extLen += 4;
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
#if COLOR_RGB_SUPPORT
	u8 hue = pScene->extField[extLen+3];
	u8 saturation = pScene->extField[extLen+4];
	extLen += 5;
#elif COLOR_CCT_SUPPORT
	u16 colorTemperatureMireds = BUILD_U16(pScene->extField[extLen+3], pScene->extField[extLen+4]);
	extLen += 5;
#endif
#endif

#ifdef ZCL_LEVEL_CTRL
	moveToLvl_t moveToLevel;
	moveToLevel.level = level;
	moveToLevel.transitionTime = pScene->transTime;
	moveToLevel.optPresent = 0;

	tuyaLight_levelCb(pAddrInfo, ZCL_CMD_LEVEL_MOVE_TO_LEVEL, &moveToLevel);
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
#if COLOR_RGB_SUPPORT
	zcl_colorCtrlMoveToHueAndSaturationCmd_t move2HueAndSat;
	move2HueAndSat.hue = hue;
	move2HueAndSat.saturation = saturation;
	move2HueAndSat.transitionTime = pScene->transTime;
	move2HueAndSat.optPresent = 0;

	tuyaLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION, &move2HueAndSat);
#elif COLOR_CCT_SUPPORT
	zcl_colorCtrlMoveToColorTemperatureCmd_t move2ColorTemp;
	move2ColorTemp.colorTemperature = colorTemperatureMireds;
	move2ColorTemp.transitionTime = pScene->transTime;
	move2ColorTemp.optPresent = 0;

	tuyaLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE, &move2ColorTemp);
#endif
#endif
}

/*********************************************************************
 * @fn      tuyaLight_sceneStoreReqHandler
 *
 * @brief   Handler for ZCL scene store command. This function will set scene attribute first.
 *
 * @param   pApsdeInd
 * @param   pScene
 *
 * @return  None
 */
static void tuyaLight_sceneStoreReqHandler(zcl_sceneEntry_t *pScene)
{
	/* receive Store Scene Request command, get the latest Scene info to save */
	u8 extLen = 0;

#ifdef ZCL_ON_OFF
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = 1;
	pScene->extField[extLen++] = pOnOff->onOff;
#endif

#ifdef ZCL_LEVEL_CTRL
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
	pScene->extField[extLen++] = 1;
	pScene->extField[extLen++] = pLevel->curLevel;
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
#if COLOR_RGB_SUPPORT
	pScene->extField[extLen++] = 2;
	pScene->extField[extLen++] = pColor->currentHue;
	pScene->extField[extLen++] = pColor->currentSaturation;
#elif COLOR_CCT_SUPPORT
	pScene->extField[extLen++] = 2;
	pScene->extField[extLen++] = LO_UINT16(pColor->colorTemperatureMireds);
	pScene->extField[extLen++] = HI_UINT16(pColor->colorTemperatureMireds);
#endif
#endif

	pScene->extFieldLen = extLen;
}

/*********************************************************************
 * @fn      tuyaLight_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t tuyaLight_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == TUYA_LIGHT_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_STORE_SCENE:
					tuyaLight_sceneStoreReqHandler((zcl_sceneEntry_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_RECALL_SCENE:
					tuyaLight_sceneRecallReqHandler(pAddrInfo, (zcl_sceneEntry_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
