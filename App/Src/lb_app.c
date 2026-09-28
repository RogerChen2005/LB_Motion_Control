/* File: lb_app.c
 * Purpose: Schedule control, input, display, and hardware motion modules.
 */
#include "lb_app.h"
#include "board_clock.h"
#include "control.h"
#include "display_panel.h"
#include "keypad.h"
#include "motion_pwm.h"
#include "serial_diag.h"
#include "touch_input.h"
#include "ui.h"
#include "SYSTEM/delay/delay.h"

static Control control;
static volatile bool started;
static volatile uint32_t heartbeat;
static uint8_t tick_div;

void LB_MotorOff(void) { Motion_EmergencyOff(); }

void LB_Tick1ms(void)
{
    if (!started) return;
    uint32_t keys;
    if (Keypad_Scan1ms(&keys)) {
        uint32_t touch=TouchInput_Mask(HAL_GetTick());
        /* Simultaneous devices are handled as a multi-key fault. */
        Control_Keys(&control,keys && touch ? 3U : keys|touch);
    }
    if (++tick_div==10U) {
        tick_div=0;
        if ((uint32_t)(HAL_GetTick()-heartbeat)>250U) control.ready=false;
        Control_Tick10ms(&control);
        Motion_Apply(control.output_reverse,Control_FrequencyMilliHz(&control));
    }
}

void LB_Init(void)
{
    BoardClock_Configure();
    delay_init(72);
    Motion_Init();
    Keypad_Init();
    SerialDiag_Init();
    bool panel=DisplayPanel_Init();
    Control_Init(&control,panel);
    SerialDiag_ReportDisplay(DisplayPanel_Id(),panel);
    if (panel) UI_Draw(&control,true);
    /* Require all input devices to be released before accepting motion. */
    control.locked=true;
    heartbeat=HAL_GetTick();
    started=true;
}

void LB_Poll(void)
{
    static uint32_t previous;
    uint32_t now=HAL_GetTick();
    heartbeat=now;
    Control snapshot;
    uint32_t primask=__get_PRIMASK();
    __disable_irq(); snapshot=control; __set_PRIMASK(primask);
    uint16_t id=DisplayPanel_Id();
    if (id!=0x9341 && id!=0x8552) return;
    TouchInput_Poll(&snapshot,now);
    if ((uint32_t)(now-previous)>=100U) {
        previous=now;
        UI_Draw(&snapshot,false);
    }
}
