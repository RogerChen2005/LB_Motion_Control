/* File: lb_app.h
 * Purpose: Declares application startup, polling, SysTick scheduling, and motor shutdown entry points.
 */
#ifndef LB_APP_H
#define LB_APP_H
#include "stm32f1xx_hal.h"
void LB_Init(void);
void LB_Poll(void);
void LB_Tick1ms(void);
void LB_MotorOff(void);
#endif
