/* File: display_panel.h
 * Purpose: Initialize the official ATK-MD0280 LCD driver and expose its chip ID.
 */
#ifndef LB_DISPLAY_PANEL_H
#define LB_DISPLAY_PANEL_H
#include <stdbool.h>
#include <stdint.h>
bool DisplayPanel_Init(void);
uint16_t DisplayPanel_Id(void);
#endif
