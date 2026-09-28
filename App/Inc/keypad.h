/* File: keypad.h
 * Purpose: Scan the external 5-by-4 switch matrix using configurable GPIO pins.
 */
#ifndef LB_KEYPAD_H
#define LB_KEYPAD_H
#include <stdbool.h>
#include <stdint.h>
void Keypad_Init(void);
/* Called each 1 ms; returns true with a fresh 20-key bitmap every fifth call. */
bool Keypad_Scan1ms(uint32_t *mask);
#endif
