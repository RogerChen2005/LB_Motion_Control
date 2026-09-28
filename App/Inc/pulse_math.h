/* File: pulse_math.h
 * Purpose: Converts a requested pulse frequency into 16-bit STM32 timer divider values.
 */
#ifndef PULSE_MATH_H
#define PULSE_MATH_H
#include <stdint.h>
/* Rounded frequency with a variable divider, supporting very slow pulses. */
static inline void Pulse_Calculate(uint32_t clock, uint32_t millihz,
                                   uint16_t *psc, uint16_t *arr)
{
    uint64_t numerator=(uint64_t)clock*1000U;
    uint64_t divisor=(uint64_t)millihz*65536U;
    uint64_t divider=(numerator+divisor-1U)/divisor;
    if (divider<1) divider=1;
    if (divider>65536) divider=65536;
    uint64_t denominator=(uint64_t)millihz*divider;
    uint64_t ticks=(numerator+denominator/2)/denominator;
    if (ticks<2) ticks=2;
    if (ticks>65536) ticks=65536;
    *psc=(uint16_t)(divider-1); *arr=(uint16_t)(ticks-1);
}
#endif
