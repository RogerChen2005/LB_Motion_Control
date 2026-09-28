/* File: board_clock.c
 * Purpose: Set HSE, PLL, and APB prescalers before application peripherals start.
 */
#include "board_clock.h"
#include "main.h"

void BoardClock_Configure(void)
{
    RCC_OscInitTypeDef oscillator={0}; RCC_ClkInitTypeDef bus={0};
    oscillator.OscillatorType=RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState=RCC_HSE_ON;
    oscillator.PLL.PLLState=RCC_PLL_ON;
    oscillator.PLL.PLLSource=RCC_PLLSOURCE_HSE;
    oscillator.PLL.PLLMUL=RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&oscillator)!=HAL_OK) Error_Handler();
    bus.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|
                  RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    bus.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
    bus.AHBCLKDivider=RCC_SYSCLK_DIV1;
    bus.APB1CLKDivider=RCC_HCLK_DIV2;
    bus.APB2CLKDivider=RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&bus,FLASH_LATENCY_2)!=HAL_OK) Error_Handler();
}
