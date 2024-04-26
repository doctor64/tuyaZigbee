/********************************************************************************************************
 * @file    bootloader.c
 *
 * @brief   This is the source file for bootloader
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

#if (__PROJECT_TL_BOOT_LOADER__)

#include "tl_common.h"
#include "bootloader.h"

/* FLASH address */
#define APP_RUNNING_ADDR					APP_IMAGE_ADDR
#define APP_NEW_IMAGE_ADDR					FLASH_ADDR_OF_OTA_IMAGE

/* SRAM address */
#if defined(MCU_CORE_826x)
	#define MCU_RAM_START_ADDR				0x808000
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define MCU_RAM_START_ADDR				0x840000
#elif defined(MCU_CORE_B91)
	#define MCU_RAM_START_ADDR				0x0000
#endif

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define REBOOT()						WRITE_REG8(0x602, 0x88)
#elif defined(MCU_CORE_B91)
	#define REBOOT()						((void(*)(void))(FLASH_R_BASE_ADDR + APP_IMAGE_ADDR))()
#endif

#define FW_START_UP_FLAG					0x4B
#define FW_RAMCODE_SIZE_MAX					RESV_FOR_APP_RAM_CODE_SIZE
#define FW_START_UP_FLAG_WHOLE			    0x544c4e4b

/* UART */
#if UART_ENABLE
#define UART_TX_BUF_SIZE    				64
#define UART_RX_BUF_SIZE    				64

#define MSG_START_FLAG						0x55
#define MSG_END_FLAG						0xAA

#define MSG_CMD_OTA_START_REQUEST			0x0210
#define MSG_CMD_OTA_START_RESPONSE			0x8210
#define MSG_CMD_OTA_BLOCK_RESPONSE			0x0211
#define MSG_CMD_OTA_BLOCK_REQUEST			0x8211
#define MSG_CMD_OTA_END_STATUS				0x8212
#define MSG_CMD_ACKNOWLEDGE					0x8000

#define MSG_BLOCK_REQUEST_INTERVAL			10//ms
#define MSG_BLOCK_REQUEST_RETRY_INTERVAL	500//ms
#define MSG_BLOCK_REQUEST_RETRY_MAX			5
#define MSG_BLOCK_REQUEST_SIZE_MAX			40

typedef enum{
	MSG_STA_SUCCESS,

	MSG_STA_OTA_GET_BLOCK_TIMEOUT,
	MSG_STA_OTA_IN_PROGRESS,
	MSG_STA_OTA_INCORRECT_OFFSET,
	MSG_STA_OTA_FILE_OVERSIZE,
	MSG_STA_OTA_ERR_BLOCK_RSP,

	MSG_STA_ERROR_START_CHAR	= 0xE0,
	MSG_STA_ERROR_END_CHAR		= 0xE2,
	MSG_STA_BAD_MSG				= 0xE3,
	MSG_STA_CRC_ERROR			= 0xE5,
}msg_status_e;

typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

typedef struct{
	u8  startFlag;
	u8	msgType16H;
	u8	msgType16L;
	u8	msgLen16H;
	u8	msgLen16L;
	u8	checkSum;
	u8	pData[1];
}uart_msg_t;

typedef struct{
	ev_timer_event_t *upgradeTimer;
	u32 flashStartAddr;
	u32 totalImageSize;
	u32 offset;
	u8	retryCnt;
}upgradeInfo_t;

__attribute__((aligned(4))) u8 uartTxBuf[UART_TX_BUF_SIZE] = {0};
__attribute__((aligned(4))) u8 uartRxBuf[UART_RX_BUF_SIZE] = {0};

static ev_queue_t msgQ;
static upgradeInfo_t upgradeInfo;
#endif	/* UART_ENABLE */

static ev_timer_event_t *otaChkTimerEvt = NULL;
static bool noAppFlg = FALSE;

static bool is_valid_fw_bootloader(u32 addr_fw){
	u32 startup_flag = 0;
    flash_read(addr_fw + FLASH_TLNK_FLAG_OFFSET, 4, (u8 *)&startup_flag);

    return ((startup_flag == FW_START_UP_FLAG_WHOLE) ? TRUE : FALSE);
}

void bootloader_with_ota_check(u32 addr_load, u32 new_image_addr){
	drv_disable_irq();

	if(new_image_addr != addr_load){
		if(is_valid_fw_bootloader(new_image_addr)){
			bool isNewImageValid = FALSE;

			u32 bufCache[256/4];  //align-4
			u8 *buf = (u8 *)bufCache;

			flash_read(new_image_addr, 256, buf);
			u32 fw_size = *(u32 *)(buf + 0x18);

			if(fw_size <= FLASH_OTA_IMAGE_MAX_SIZE){
				s32 totalLen = fw_size - 4;
				u32 wLen = 0;
				u32 sAddr = new_image_addr;

				u32 crcVal = 0;
				flash_read(new_image_addr + fw_size - 4, 4, (u8 *)&crcVal);

				u32 curCRC = 0xffffffff;

				while(totalLen > 0){
					wLen = (totalLen > 256) ? 256 : totalLen;
					flash_read(sAddr, wLen, buf);
					curCRC = xcrc32(buf, wLen, curCRC);

					totalLen -= wLen;
					sAddr += wLen;
				}

				if(curCRC == crcVal){
					isNewImageValid = TRUE;
				}
			}

			if(isNewImageValid){
				u8 readBuf[256];

				for(int i = 0; i < fw_size; i += 256){
					if((i & 0xfff) == 0){
						flash_erase(addr_load + i);
					}

					flash_read(new_image_addr + i, 256, buf);
					flash_write(addr_load + i, 256, buf);

					flash_read(addr_load + i, 256, readBuf);
					if(memcmp(readBuf, buf, 256)){
						SYSTEM_RESET();
					}

					gpio_toggle(LED_PERMIT);
				}
			}

			buf[0] = 0;
			flash_write(new_image_addr + FLASH_TLNK_FLAG_OFFSET, 1, buf);   //clear OTA flag

			//erase the new firmware
			for(int i = 0; i < ((fw_size + 4095)/4096); i++) {
				flash_erase(new_image_addr + i*4096);
			}
		}
	}

    if(is_valid_fw_bootloader(addr_load)){
#if !defined(MCU_CORE_B91)
    	u32 ramcode_size = 0;
        flash_read(addr_load + 0x0c, 2, (u8 *)&ramcode_size);
        ramcode_size *= 16;

        if(ramcode_size > FW_RAMCODE_SIZE_MAX){
            ramcode_size = FW_RAMCODE_SIZE_MAX; // error, should not run here
        }
        flash_read(addr_load, ramcode_size, (u8 *)MCU_RAM_START_ADDR); // copy ram code
#endif
        REBOOT();
    }else{
    	noAppFlg = TRUE;
    }
}



s32 otaChkDelayCb(void *arg){
	bootloader_with_ota_check(APP_RUNNING_ADDR, APP_NEW_IMAGE_ADDR);

	otaChkTimerEvt = NULL;
	return -1;
}

void bootloader_ota_check_delay(u32 delayMs){
	if(delayMs){
		if(otaChkTimerEvt){
			TL_ZB_TIMER_CANCEL(&otaChkTimerEvt);
		}
		otaChkTimerEvt = TL_ZB_TIMER_SCHEDULE(otaChkDelayCb, NULL, delayMs);
	}else{
		bootloader_with_ota_check(APP_RUNNING_ADDR, APP_NEW_IMAGE_ADDR);
	}
}

void bootloader_ota_check_Stop(void){
	if(otaChkTimerEvt){
		TL_ZB_TIMER_CANCEL(&otaChkTimerEvt);
	}
}

#if UART_ENABLE
u8 crc8Calc(u16 type, u16 len, u8 *data){
	u8 crc8;

	crc8  = (type >> 0) & 0xff;
	crc8 ^= (type >> 8) & 0xff;
	crc8 ^= (len >> 0) & 0xff;
	crc8 ^= (len >> 8) & 0xff;

	for(u16 i = 0; i < len; i++){
		crc8 ^= data[i];
	}

	return crc8;
}

void bootloader_uartTx(u16 type, u16 len, u8 *data){
    u8 crc8 = crc8Calc(type, len, data);

    u8 *pData = uartTxBuf;

    *pData++ = MSG_START_FLAG;
    *pData++ = (type >> 8) & 0xff;
    *pData++ = (type >> 0) & 0xff;
    *pData++ = (len >> 8) & 0xff;
    *pData++ = (len >> 0) & 0xff;
    *pData++ = crc8;
    for(u16 i = 0; i < len; i++){
        *pData++ = data[i];
    }
    *pData++ = MSG_END_FLAG;

    drv_uart_tx_start(uartTxBuf, pData - uartTxBuf);
}

void bootloader_uartAck(u16 type, u8 status){
	u8 array[4] = {0};

	array[0] = (type >> 8) & 0xff;
	array[1] = (type >> 0) & 0xff;
	array[2] = status;
	array[3] = 0;

	bootloader_uartTx(MSG_CMD_ACKNOWLEDGE, 4, array);
}

void bootloader_uartOtaStartRsp(u8 status){
	u8 array[16] = {0};
	u8 *pBuf = array;

	*pBuf++ = (upgradeInfo.flashStartAddr >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.flashStartAddr >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.flashStartAddr >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.flashStartAddr >> 0) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 0) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 0) & 0xff;
	*pBuf++ = status;

	bootloader_uartTx(MSG_CMD_OTA_START_RESPONSE, pBuf - array, array);
}

void bootloader_uartOtaEnd(u8 status){
	u8 array[16] = {0};
	u8 *pBuf = array;

	*pBuf++ = (upgradeInfo.totalImageSize >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.totalImageSize >> 0) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 0) & 0xff;
	*pBuf++ = status;

	bootloader_uartTx(MSG_CMD_OTA_END_STATUS, pBuf - array, array);
}

void bootloader_uartBlockReq(void){
	u8 array[6] = {0};
	u8 *pBuf = array;

	u8 blockReqLen = 0;

	if(upgradeInfo.totalImageSize - upgradeInfo.offset >= MSG_BLOCK_REQUEST_SIZE_MAX){
		blockReqLen = MSG_BLOCK_REQUEST_SIZE_MAX;
	}else{
		blockReqLen = upgradeInfo.totalImageSize - upgradeInfo.offset;
	}

	*pBuf++ = (upgradeInfo.offset >> 24) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 16) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 8) & 0xff;
	*pBuf++ = (upgradeInfo.offset >> 0) & 0xff;
	*pBuf++ = blockReqLen;

	bootloader_uartTx(MSG_CMD_OTA_BLOCK_REQUEST, pBuf - array, array);
}

void bootloader_uartOtaComplete(u8 status){
	bootloader_uartOtaEnd(status);

	if(upgradeInfo.upgradeTimer){
		TL_ZB_TIMER_CANCEL(&upgradeInfo.upgradeTimer);
	}
	memset((u8 *)&upgradeInfo, 0, sizeof(upgradeInfo_t));
}

s32 upgradeBlockReqTimerCb(void *arg){
	if(upgradeInfo.retryCnt++ < MSG_BLOCK_REQUEST_RETRY_MAX){
		bootloader_uartBlockReq();

		return MSG_BLOCK_REQUEST_RETRY_INTERVAL;
	}else{
		//upgrade fail.
		bootloader_uartOtaEnd(MSG_STA_OTA_GET_BLOCK_TIMEOUT);

		memset((u8 *)&upgradeInfo, 0, sizeof(upgradeInfo_t));

		upgradeInfo.upgradeTimer = NULL;
		return -1;
	}
}

void bootloader_upgrade(u16 type, u16 len, u8 *data){
	u8 sta = MSG_STA_SUCCESS;
	u8 *pData = data;

	switch(type){
		case MSG_CMD_OTA_START_REQUEST:
			if(upgradeInfo.flashStartAddr == 0){
				u32 totalImageSize = (pData[0] << 24)
								   + (pData[1] << 16)
								   + (pData[2] << 8)
								   + (pData[3]);

				if(totalImageSize <= FLASH_OTA_IMAGE_MAX_SIZE){
					//received upgrade start message, stop the timer.
					bootloader_ota_check_Stop();

					upgradeInfo.retryCnt = 0;
					upgradeInfo.offset = 0;
					upgradeInfo.flashStartAddr = APP_NEW_IMAGE_ADDR;
					upgradeInfo.totalImageSize = totalImageSize;

					//erase the new image area.
					for(u32 i = 0; i < ((upgradeInfo.totalImageSize + 4095) / 4096); i++) {
						flash_erase(APP_NEW_IMAGE_ADDR + i * 4096);
					}

					if(upgradeInfo.upgradeTimer){
						TL_ZB_TIMER_CANCEL(&upgradeInfo.upgradeTimer);
					}
					upgradeInfo.upgradeTimer = TL_ZB_TIMER_SCHEDULE(upgradeBlockReqTimerCb,
																	NULL,
																	MSG_BLOCK_REQUEST_INTERVAL);
				}else{
					sta = MSG_STA_OTA_FILE_OVERSIZE;
				}
			}else{
				sta = MSG_STA_OTA_IN_PROGRESS;
			}

			bootloader_uartOtaStartRsp(sta);
			break;
		case MSG_CMD_OTA_BLOCK_RESPONSE:
			{
				if(upgradeInfo.upgradeTimer){
					TL_ZB_TIMER_CANCEL(&upgradeInfo.upgradeTimer);
				}

				u8 rspStatus = *pData++;

				if((rspStatus != MSG_STA_SUCCESS) || (upgradeInfo.totalImageSize == 0)){
					bootloader_uartOtaComplete(MSG_STA_OTA_ERR_BLOCK_RSP);
					return;
				}else{
					u32 rcvOffset = (pData[0] << 24)
								  + (pData[1] << 16)
								  + (pData[2] << 8)
								  + (pData[3]);
					pData += 4;
					u8 rcvBlockLen = *pData++;

					if((rcvOffset == upgradeInfo.offset) &&
					   (rcvBlockLen && (rcvBlockLen <= MSG_BLOCK_REQUEST_SIZE_MAX))){
						//disable boot-up flag
						if(rcvOffset == 0){
							pData[FLASH_TLNK_FLAG_OFFSET] = 0xff;
						}

						flash_write(upgradeInfo.flashStartAddr + rcvOffset, rcvBlockLen, pData);

						upgradeInfo.offset += rcvBlockLen;

						if(upgradeInfo.offset == upgradeInfo.totalImageSize){
							//upgrade success, enable boot-up flag
							u8 startUpFlg = FW_START_UP_FLAG;
							flash_write((upgradeInfo.flashStartAddr + FLASH_TLNK_FLAG_OFFSET), 1, &startUpFlg);

							bootloader_ota_check_delay(50);

							bootloader_uartOtaComplete(MSG_STA_SUCCESS);
							return;
						}else if(upgradeInfo.offset > upgradeInfo.totalImageSize){
							bootloader_uartOtaComplete(MSG_STA_OTA_INCORRECT_OFFSET);
							return;
						}else if(upgradeInfo.offset < upgradeInfo.totalImageSize){
							upgradeInfo.retryCnt = 0;
							upgradeInfo.upgradeTimer = TL_ZB_TIMER_SCHEDULE(upgradeBlockReqTimerCb,
																			NULL,
																			MSG_BLOCK_REQUEST_INTERVAL);
						}
					}else{
						sta = MSG_STA_OTA_INCORRECT_OFFSET;
					}

					bootloader_uartAck(type, sta);
				}
			}
			break;
		default:
			break;
	}
}

void bootloader_uartRxDataProc(void){
	/* process messages in the uart ISR, and we must check the uart RX state in main loop. */
	drv_uart_exceptionProcess();

	u8 *buf = ev_queue_pop(&msgQ);
	if(buf){
		gpio_toggle(LED_POWER);

		uart_msg_t *pMsg = (uart_msg_t *)buf;

		u8 sta = MSG_STA_SUCCESS;
		u16 msgType = 0xffff;
		u16 msgLen = 0;

		if(pMsg->startFlag == MSG_START_FLAG){
			msgType = (pMsg->msgType16H << 8) + pMsg->msgType16L;
			msgLen = (pMsg->msgLen16H << 8) + pMsg->msgLen16L;

			if(*(pMsg->pData + msgLen) == MSG_END_FLAG){
				if((msgType == MSG_CMD_OTA_START_REQUEST) || (msgType == MSG_CMD_OTA_BLOCK_RESPONSE)){
					u8 crc8 = crc8Calc(msgType, msgLen, pMsg->pData);

					if(pMsg->checkSum == crc8){
						bootloader_upgrade(msgType, msgLen, pMsg->pData);
					}else{
						sta = MSG_STA_CRC_ERROR;
					}
				}else{
					sta = MSG_STA_BAD_MSG;
				}
			}else{
				sta = MSG_STA_ERROR_END_CHAR;
			}
		}else{
			sta = MSG_STA_ERROR_START_CHAR;
		}

		if(sta != MSG_STA_SUCCESS){
			bootloader_uartAck(msgType, sta);
		}

		ev_buf_free(buf);
	}
}

void bootloader_uartRxHandler(void){
	uart_rxData_t *rxData = (uart_rxData_t *)uartRxBuf;
	if(rxData->dataLen && (rxData->dataLen <= (UART_RX_BUF_SIZE - 4))){
		u8 *buf = ev_buf_allocate(rxData->dataLen);
		if(buf){
			memcpy(buf, rxData->dataPayload, rxData->dataLen);

			ev_queue_push(&msgQ, buf);
		}
	}
}

void bootloader_keyPressedCb(kb_data_t *kbEvt){
	u8 keyCode = kbEvt->keycode[0];

	if(keyCode == VK_SW1){
		//cancel the timeout timer, wait for UART upgrade message.
		bootloader_ota_check_Stop();
	}
}

void bootloader_keyPressProc(void){
	if(kb_scan_key(0 , 1)){
		if(kb_event.cnt){
			bootloader_keyPressedCb(&kb_event);
		}
	}
}
#endif


void bootloader_init(bool isBoot){
	if(isBoot){
		drv_gpio_write(LED_POWER, 1);
		drv_gpio_write(LED_PERMIT, 1);

#if UART_ENABLE
		drv_enable_irq();

		UART_PIN_CFG();
		drv_uart_init(115200, uartRxBuf, UART_RX_BUF_SIZE, bootloader_uartRxHandler);

		ev_queue_init(&msgQ, NULL);
		ev_on_poll(EV_POLL_UART_PROC, bootloader_uartRxDataProc);

		ev_on_poll(EV_POLL_KEY_PRESS, bootloader_keyPressProc);

		memset((u8 *)&upgradeInfo, 0, sizeof(upgradeInfo_t));

		//start a timer delay for waiting for uart messages.
		bootloader_ota_check_delay(2000);
#else
		bootloader_ota_check_delay(0);
#endif
	}else{
		bootloader_ota_check_delay(0);
	}
}

void bootloader_loop(void){
	if(noAppFlg){
		gpio_toggle(LED_POWER);
		gpio_toggle(LED_PERMIT);
		WaitMs(100);
	}
}

#endif	/* __PROJECT_TL_BOOT_LOADER__ */
