/* File: control.c
 * Purpose: Handles keypad debounce, speed entry, run and jog states, and acceleration targets.
 */
#include "control.h"
#include <string.h>

void Control_Init(Control *c, bool ready)
{
    memset(c, 0, sizeof(*c));
    c->set_speed=DEFAULT_SPEED_MM_MIN; c->key=K_NONE; c->candidate=K_NONE; c->ready=ready;
}
static void stop(Control *c) { c->run=false; c->jog=false; }
static bool idle(const Control *c) { return !c->run && !c->jog && c->speed_milli==0; }
static void press(Control *c, uint8_t k)
{
    static const int8_t number[20]={-1,-1,-1,-1,1,2,3,-1,4,5,6,-1,7,8,9,-1,-1,0,-1,-1};
    if (k==K_ESC || k==K_F2) {
        stop(c); c->editing=false; c->digits=0; c->input=0; c->input_error=false; return;
    }
    if (k==K_F1) {
        if (c->run) stop(c);
        else if (idle(c) && !c->editing && c->ready && c->set_speed) c->run=true;
        return;
    }
    if (k==K_UP || k==K_DOWN) {
        if (c->ready && !c->editing) {
            c->run=false; c->reverse=(k==K_DOWN); c->jog=true;
        }
        return;
    }
    if (k==K_LEFT || k==K_RIGHT) {
        if (!c->editing) c->reverse=(k==K_LEFT);
        return;
    }
    if (!idle(c)) return; /* Speed entry still requires a complete stop. */
    if (number[k]>=0) {
        if (!c->editing) { c->input=0; c->digits=0; c->editing=true; }
        if (c->digits<5) { c->input=c->input*10U+(uint32_t)number[k]; c->digits++; }
        c->input_error=c->input>MAX_SPEED_MM_MIN; return;
    }
    if (k==K_HASH) { c->input=0; c->digits=0; c->editing=true; c->input_error=false; }
    if (k==K_STAR && c->editing) {
        c->input/=10; if (c->digits) c->digits--; c->input_error=c->input>MAX_SPEED_MM_MIN;
    }
    if (k==K_ENTER && c->editing && c->digits) {
        if (c->input>MAX_SPEED_MM_MIN) { c->input_error=true; return; }
        c->set_speed=c->input; c->editing=false; c->digits=0; c->input_error=false;
        if (ENTER_STARTS_RUN && c->ready && c->set_speed) c->run=true;
    }
}
void Control_Keys(Control *c, uint32_t mask)
{
    uint8_t raw=K_NONE;
    mask &= 0xFFFFFU;
    if (mask && (mask & (mask-1U))) {
        stop(c); c->locked=true; c->debounce=0; c->candidate=K_NONE; c->key=K_NONE; return;
    }
    if (mask) { raw=0; while ((mask & (1UL<<raw))==0) raw++; }
    /* A released jog key stops requesting motion immediately, without waiting
       for release debounce. A new press still requires 20 ms stability. */
    if (c->jog && raw!=c->key) c->jog=false;
    if (raw!=c->candidate) { c->candidate=raw; c->debounce=1; }
    else if (c->debounce<4) c->debounce++;
    if (c->debounce<4) return;
    if (c->locked) {
        if (raw==K_NONE) { c->locked=false; c->key=K_NONE; }
        return;
    }
    if (c->key==raw) return;
    c->key=raw;
    if (raw!=K_NONE) press(c, raw);
}
void Control_Tick10ms(Control *c)
{
    uint32_t target=0, step=ACCEL_MILLI_SPEED_PER_MS*10U;
    if (!c->ready || c->locked) stop(c);
    if (c->jog) target=JOG_SPEED_MM_MIN*1000U;
    else if (c->run) target=c->set_speed*1000U;
    if (c->reverse!=c->output_reverse) {
        target=0;
        if (c->speed_milli==0) { c->output_reverse=c->reverse; c->reversal_wait=2; }
    }
    if (c->reversal_wait) { c->reversal_wait--; target=0; }
    if (c->speed_milli<target) {
        uint32_t delta=target-c->speed_milli; c->speed_milli+=delta<step ? delta : step;
    } else if (c->speed_milli>target) {
        uint32_t delta=c->speed_milli-target; c->speed_milli-=delta<step ? delta : step;
    }
}
uint32_t Control_FrequencyMilliHz(const Control *c)
{
    return (uint32_t)((uint64_t)c->speed_milli*PULSES_PER_REV*1000U/(60U*LEAD_UM_PER_REV));
}
