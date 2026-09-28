/* File: board_pins.c
 * Purpose: Enables a GPIO port selected in board_pins.h.
 */
#include "board_pins.h"

void Board_EnableGpioClock(GPIO_TypeDef *port)
{
    if (port==GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port==GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port==GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port==GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port==GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (port==GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
    else if (port==GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();
}
