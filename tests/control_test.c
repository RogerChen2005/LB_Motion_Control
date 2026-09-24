/* File: control_test.c
 * Purpose: Checks speed entry, jog release, motion states, and timer-frequency calculations on a host.
 */
#include "control.h"
#include "pulse_math.h"
#include <assert.h>
#include <stdio.h>
static void frames(Control *c,uint32_t mask,int n) { while(n--) Control_Keys(c,mask); }
static void key(Control *c,int k) { frames(c,1UL<<k,4); frames(c,0,4); }
static void ticks(Control *c,int n) { while(n--) Control_Tick10ms(c); }
int main(void)
{
    Control c; Control_Init(&c,true);
    key(&c,K_2); key(&c,K_4); key(&c,K_0); key(&c,K_ENTER);
    assert(c.set_speed==240 && !c.run && !c.editing);
    key(&c,K_F1); ticks(&c,100);
    assert(c.run && c.speed_milli==240000);
    assert(Control_FrequencyMilliHz(&c)==1600000); /* 240mm/min, 4mm, 1600p/rev */
    key(&c,K_LEFT); assert(!c.reverse); /* Cannot reverse while moving. */
    key(&c,K_F1); ticks(&c,100); assert(!c.run && !c.speed_milli);
    key(&c,K_F1); ticks(&c,1); assert(c.run && c.speed_milli); /* restart */
    key(&c,K_ESC); ticks(&c,100);
    frames(&c,1UL<<K_DOWN,3); assert(!c.jog);
    frames(&c,1UL<<K_DOWN,1); assert(c.jog);
    ticks(&c,1); assert(c.output_reverse && !c.speed_milli);
    ticks(&c,1); assert(!c.speed_milli); ticks(&c,101); assert(c.speed_milli==480000);
    frames(&c,0,1); assert(!c.jog); ticks(&c,100); assert(!c.speed_milli);
    frames(&c,0,4); frames(&c,1UL<<K_UP,4); ticks(&c,50); assert(c.jog);
    frames(&c,(1UL<<K_UP)|(1UL<<K_DOWN),1); assert(c.locked && !c.jog && !c.run);
    frames(&c,1UL<<K_UP,10); assert(c.locked && !c.jog);
    frames(&c,0,4); ticks(&c,120); assert(!c.locked && !c.speed_milli);
    key(&c,K_9); key(&c,K_9); key(&c,K_9); key(&c,K_9); key(&c,K_ENTER);
    assert(c.input_error && c.editing && c.set_speed==240);
    key(&c,K_STAR); key(&c,K_ENTER); assert(c.set_speed==999 && !c.editing);
    key(&c,K_HASH); key(&c,K_0); key(&c,K_ENTER); key(&c,K_F1); assert(!c.run);
    key(&c,K_6); key(&c,K_0); key(&c,K_ENTER); key(&c,K_F1);
    c.ready=false; ticks(&c,100); assert(!c.run && !c.speed_milli);
    Control_Init(&c,true); c.locked=true;
    frames(&c,1UL<<K_UP,20); assert(!c.jog); frames(&c,0,4);
    frames(&c,1UL<<K_UP,4); assert(c.jog);
    for (unsigned speed=1;speed<=MAX_SPEED_MM_MIN;speed++) {
        c.speed_milli=speed*1000U; uint32_t mhz=Control_FrequencyMilliHz(&c);
        uint16_t p,a; Pulse_Calculate(72000000,mhz,&p,&a);
        double actual=72000000000.0/((p+1.0)*(a+1.0));
        double error=(actual-mhz)/mhz; assert(error<0.001 && error> -0.001);
        assert((a+1.0)*(p+1.0)/72000000.0/2.0>0.0000025);
    }
    puts("PASS: input/save, run/restart/stop, jog/release, debounce, reversal, multi-key, range, boot lock, units, PWM range.");
}
