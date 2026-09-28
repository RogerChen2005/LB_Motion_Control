/* File: delay.h
 * Purpose: Declares timing helpers expected by the official LCD and touch drivers.
 */
#ifndef LB_DELAY_H
#define LB_DELAY_H
#include <stdint.h>
void delay_init(uint8_t mhz);
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);
#endif
