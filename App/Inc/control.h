/* File: control.h
 * Purpose: Defines zero-based keypad codes and the motion-control state machine interface.
 */
#ifndef CONTROL_H
#define CONTROL_H
#include <stdbool.h>
#include <stdint.h>

/* Key numbers follow the physical keyboard: 5 rows, 4 columns. */
enum { K_F1=0, K_F2, K_HASH, K_STAR, K_1, K_2, K_3, K_UP,
       K_4, K_5, K_6, K_DOWN, K_7, K_8, K_9, K_ESC,
       K_LEFT, K_0, K_RIGHT, K_ENTER, K_NONE=255 };
#include "machine_config.h"

typedef struct {
    uint32_t set_speed, input, speed_milli;
    uint8_t digits, key, candidate, debounce;
    bool reverse, output_reverse, run, jog, editing, locked, ready;
    uint8_t reversal_wait;
    bool input_error;
} Control;
void Control_Init(Control *c, bool ready);
/* One complete 20-key raw bitmap every 5 ms. Multiple keys stop motion. */
void Control_Keys(Control *c, uint32_t mask);
void Control_Tick10ms(Control *c);
uint32_t Control_FrequencyMilliHz(const Control *c);
#endif
