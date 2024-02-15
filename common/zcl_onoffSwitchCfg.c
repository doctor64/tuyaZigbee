/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"

#ifdef ZCL_ON_OFF_SWITCH_CFG
_CODE_ZCL_ status_t zcl_onOffSwitchCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
//	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);
	u8 status = ZCL_STA_SUCCESS;
printf("switchcfg register start \n");
    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);

    if(status == ZCL_STA_SUCCESS){
printf("switchcfg register status  0x%x\n", status);
//    	zcl_level_startUpCurrentLevel(endpoint);
    }
printf("switchcfg register status 2  0x%x\n", status);
    return status;

}
#endif /* ZCL_ON_OFF_SWITCH_CFG */