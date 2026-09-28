/* File: delay.c
 * Purpose: Implements HAL-compatible millisecond and DWT microsecond delays.
 */
#include "delay.h"
#include "stm32f1xx_hal.h"
void delay_init(uint8_t mhz)
{
    (void)mhz;
    CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT=0; DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk;
}
void delay_us(uint32_t us)
{
    /* Short SPI delays only. Does not repurpose SysTick used by motion control. */
    uint32_t start=DWT->CYCCNT, cycles=us*(SystemCoreClock/1000000U);
    while ((uint32_t)(DWT->CYCCNT-start)<cycles) { __NOP(); }
}
void delay_ms(uint16_t ms) { HAL_Delay(ms); }
