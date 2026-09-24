/* File: sys.h
 * Purpose: Adapts the official ATK-MD0280 board support headers to this HAL project.
 */
#ifndef LB_SYS_COMPAT_H
#define LB_SYS_COMPAT_H
/* Compatibility layer for the original ALIENTEK BSP, using the project's HAL. */
#include "stm32f1xx_hal.h"
#ifndef __nop
#define __nop() __NOP()
#endif
#endif
