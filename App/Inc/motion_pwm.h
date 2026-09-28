/* File: motion_pwm.h
 * Purpose: Configure TIM3 hardware step pulses and DM542 direction/enable GPIOs.
 */
#ifndef LB_MOTION_PWM_H
#define LB_MOTION_PWM_H
#include <stdbool.h>
#include <stdint.h>
void Motion_Init(void);
void Motion_Apply(bool reverse,uint32_t frequency_millihz);
void Motion_EmergencyOff(void);
#endif
