/********************************************************************************************************
 * @file    zb_afTestCb.c
 *
 * @brief   This is the source file for zb_afTestCb
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
#if ZBHCI_EN
#include "zbhci.h"
#endif

#if AF_TEST_ENABLE
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
u16 g_afTest_rcvReqCnt = 0;

/**********************************************************************
 * FUNCTIONS
 */
static void afTest_testReqPrc(apsdeDataInd_t *pApsdeInd)
{
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

	u8 dataLen = 50;
	u8 *pBuf = (u8 *)ev_buf_allocate(dataLen);
	if(pBuf){
		u8 *pData = pBuf;

		*pData++ = LO_UINT16(g_afTest_rcvReqCnt);
		*pData++ = HI_UINT16(g_afTest_rcvReqCnt);

		for(u8 i = 0; i < dataLen - 2; i++){
			*pData++ = i;
		}

		u8 apsCnt = 0;
#if ZBHCI_EN
		zbhciTx(ZCL_CLUSTER_TELINK_SDK_TEST_RSP, pApsdeInd->asduLen, (u8 *)pApsdeInd->asdu);
#else
		af_dataSend(pApsdeInd->indInfo.dst_ep, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_RSP, dataLen, pBuf, &apsCnt);
#endif

		ev_buf_free(pBuf);
	}
}

static void afTest_testClearReqPrc(apsdeDataInd_t *pApsdeInd)
{
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

	u8 st = SUCCESS;

	u8 apsCnt = 0;
	af_dataSend(pApsdeInd->indInfo.dst_ep, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_RSP, 1, &st, &apsCnt);
}

void afTest_rx_handler(void *arg)
{
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)arg;

	switch(pApsdeInd->indInfo.cluster_id){
		case ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_REQ:
			g_afTest_rcvReqCnt = 0;
			afTest_testClearReqPrc(pApsdeInd);
			break;
		case ZCL_CLUSTER_TELINK_SDK_TEST_REQ:
			g_afTest_rcvReqCnt++;
			afTest_testReqPrc(pApsdeInd);
			break;
		case ZCL_CLUSTER_TELINK_SDK_TEST_RSP:

			break;
		default:
			break;
	}

	/* Must be free here. */
	ev_buf_free((u8 *)arg);
}

void afTest_dataSendConfirm(void *arg)
{
//	apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

}

#endif	/* AF_TEST_ENABLE */
#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
