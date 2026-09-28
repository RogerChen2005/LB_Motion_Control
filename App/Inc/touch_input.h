/* File: touch_input.h
 * Purpose: Translate one held LCD touch into the same key codes as the matrix.
 */
#ifndef LB_TOUCH_INPUT_H
#define LB_TOUCH_INPUT_H
#include "control.h"
#include <stdint.h>
void TouchInput_Poll(const Control *state,uint32_t now);
uint32_t TouchInput_Mask(uint32_t now);
#endif
