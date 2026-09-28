/* File: touch_input.c
 * Purpose: Poll the official touch BSP and release stale or slid-off touches.
 */
#include "touch_input.h"
#include "ui.h"
#include "BSP/ATK_MD0280/atk_md0280_touch.h"

static volatile uint32_t current_mask, last_sample;
static bool pen_held, page_editing;
static uint8_t anchor=K_NONE;
static uint32_t last_poll;

void TouchInput_Poll(const Control *state,uint32_t now)
{
    if ((uint32_t)(now-last_poll)<10U) return;
    last_poll=now;
    uint16_t x=0,y=0;
    uint8_t result=atk_md0280_touch_scan(&x,&y);
    uint8_t key=result==ATK_MD0280_TOUCH_EOK ? UI_Hit(state,x,y) : K_NONE;
    bool down=ATK_MD0280_TOUCH_READ_PEN()==0;
    if (!down) { pen_held=false; anchor=K_NONE; }
    else if (!pen_held) { pen_held=true; anchor=key; page_editing=state->editing; }
    else if (key!=anchor || page_editing!=state->editing) anchor=K_NONE;
    uint32_t mask=down && anchor!=K_NONE ? 1UL<<anchor : 0;
    uint32_t primask=__get_PRIMASK(); __disable_irq();
    current_mask=mask; last_sample=HAL_GetTick(); __set_PRIMASK(primask);
}
uint32_t TouchInput_Mask(uint32_t now)
{
    return (uint32_t)(now-last_sample)<80U ? current_mask : 0;
}
