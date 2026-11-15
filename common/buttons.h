/********************************************************************************************************
 * @file    keys.h
 *
 * @brief   Contain common buttons processing defines for all subprojects
 *
 * @author  doctor64
 * @date    2024
 *******************************************************************************************************/
#ifndef BUTTONS_H
#define BUTTONS_H

#include "types.h"

#define BUTTON_NET_LONG_PRESS_TIME 5*1000*1000
#define BUTTON_SW_LONG_PRESS_TIME  3*1000*1000
#define BUTTON_CLICK_CHECK_TIME       250*1000
#define BUTTON_DEBOUNCE_TIME           50*1000
#define BUTTON_MIN_PRESS_TIME          30*1000

enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_PRESSED_PROCESSED,
    BUTTON_STATE_LONG_RELEASED,
};
enum
{
    BUTTON_ACTION_RELEASED  = 255,   /* release after hold */
    BUTTON_ACTION_SINGLE    = 1,
    BUTTON_ACTION_DOUBLE    = 2,
    BUTTON_ACTION_TRIPLE    = 3,
    BUTTON_ACTION_PRESSED   = 0      /* long press / hold */
};
#define BUTTON_STATES_NUM     5

typedef struct {
    u8  count;
    u8  state;
    u32 press_time;
    u32 release_time;
} button_t;
  
#endif