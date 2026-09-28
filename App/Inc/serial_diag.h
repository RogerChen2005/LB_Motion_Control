/* File: serial_diag.h
 * Purpose: Send startup diagnostics over the board's fixed USART1 connection.
 */
#ifndef LB_SERIAL_DIAG_H
#define LB_SERIAL_DIAG_H
#include <stdbool.h>
#include <stdint.h>
void SerialDiag_Init(void);
void SerialDiag_ReportDisplay(uint16_t id,bool ready);
#endif
