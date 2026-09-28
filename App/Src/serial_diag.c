/* File: serial_diag.c
 * Purpose: Set up USART1 and report the detected LCD and machine configuration.
 */
#include "serial_diag.h"
#include "board_pins.h"
#include "machine_config.h"
#include "main.h"
#include <stdio.h>

static UART_HandleTypeDef debug_uart;

void SerialDiag_Init(void)
{
    GPIO_InitTypeDef gpio={0};
    Board_EnableGpioClock(LB_UART_TX_PORT);
    Board_EnableGpioClock(LB_UART_RX_PORT);
    __HAL_RCC_USART1_CLK_ENABLE();
    gpio.Pin=LB_UART_TX_PIN; gpio.Mode=GPIO_MODE_AF_PP;
    gpio.Speed=GPIO_SPEED_FREQ_HIGH; HAL_GPIO_Init(LB_UART_TX_PORT,&gpio);
    gpio.Pin=LB_UART_RX_PIN; gpio.Mode=GPIO_MODE_INPUT; gpio.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(LB_UART_RX_PORT,&gpio);
    debug_uart.Instance=LB_DEBUG_UART;
    debug_uart.Init.BaudRate=115200; debug_uart.Init.WordLength=UART_WORDLENGTH_8B;
    debug_uart.Init.StopBits=UART_STOPBITS_1; debug_uart.Init.Parity=UART_PARITY_NONE;
    debug_uart.Init.Mode=UART_MODE_TX_RX; debug_uart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    debug_uart.Init.OverSampling=UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&debug_uart)!=HAL_OK) Error_Handler();
}

void SerialDiag_ReportDisplay(uint16_t id,bool ready)
{
    char message[100];
    int n=snprintf(message,sizeof(message),
        "LB HAL: LCD=%04X, %s; PPR=%u; lead=%lu.%03lumm\r\n",
        id,ready ? "READY" : "UNSUPPORTED - MOTION BLOCKED",
        (unsigned)PULSES_PER_REV,
        (unsigned long)(LEAD_UM_PER_REV/1000U),
        (unsigned long)(LEAD_UM_PER_REV%1000U));
    if (n>0) (void)HAL_UART_Transmit(&debug_uart,(uint8_t *)message,(uint16_t)n,100);
}
