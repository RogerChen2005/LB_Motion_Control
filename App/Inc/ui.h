/* File: ui.h
 * Purpose: Declares screen rendering and touch hit testing for the controller interface.
 */
#ifndef LB_UI_H
#define LB_UI_H
#include "control.h"
void UI_Draw(const Control *s, bool force);
uint8_t UI_Hit(const Control *s, uint16_t x, uint16_t y);
#endif
