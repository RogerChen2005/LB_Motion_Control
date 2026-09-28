/* File: board_pins.h
 * Purpose: Single pin map for user wiring and the Elite board's fixed AFIO buses.
 * Edit the ordinary GPIO section for external keyboard and DM542 control.
 * Timer, USART, and FSMC pins below follow STM32F103 hardware routing.
 */
#ifndef BOARD_PINS_H
#define BOARD_PINS_H
#include "stm32f1xx_hal.h"

typedef struct { GPIO_TypeDef *port; uint16_t pin; } BoardPin;
void Board_EnableGpioClock(GPIO_TypeDef *port);

/* User-wired GPIO: port and pin can be changed as a pair. */
#define LB_DIR_PORT GPIOE
#define LB_DIR_PIN GPIO_PIN_6
#define LB_ENABLE_PORT GPIOE
#define LB_ENABLE_PIN GPIO_PIN_1

/* P1 right column: PA15, PA13, PA11, PC9, PC7, PG8, PG6, PG4, PG2.
 * Rows use the first five contacts; the user's keypad column order runs
 * from the PG2 end back toward PG8. */
#define LB_KEY_ROW0_PORT GPIOA
#define LB_KEY_ROW0_PIN GPIO_PIN_15
#define LB_KEY_ROW1_PORT GPIOA
#define LB_KEY_ROW1_PIN GPIO_PIN_13
#define LB_KEY_ROW2_PORT GPIOA
#define LB_KEY_ROW2_PIN GPIO_PIN_11
#define LB_KEY_ROW3_PORT GPIOC
#define LB_KEY_ROW3_PIN GPIO_PIN_9
#define LB_KEY_ROW4_PORT GPIOC
#define LB_KEY_ROW4_PIN GPIO_PIN_7
#define LB_KEY_COL0_PORT GPIOG
#define LB_KEY_COL0_PIN GPIO_PIN_2
#define LB_KEY_COL1_PORT GPIOG
#define LB_KEY_COL1_PIN GPIO_PIN_4
#define LB_KEY_COL2_PORT GPIOG
#define LB_KEY_COL2_PIN GPIO_PIN_6
#define LB_KEY_COL3_PORT GPIOG
#define LB_KEY_COL3_PIN GPIO_PIN_8

/* LCD backlight and resistive touch use ordinary GPIO / software SPI. */
#define LB_LCD_BL_PORT GPIOB
#define LB_LCD_BL_PIN GPIO_PIN_0
#define LB_TOUCH_PEN_PORT GPIOF
#define LB_TOUCH_PEN_PIN GPIO_PIN_10
#define LB_TOUCH_MISO_PORT GPIOB
#define LB_TOUCH_MISO_PIN GPIO_PIN_2
#define LB_TOUCH_MOSI_PORT GPIOF
#define LB_TOUCH_MOSI_PIN GPIO_PIN_9
#define LB_TOUCH_CS_PORT GPIOF
#define LB_TOUCH_CS_PIN GPIO_PIN_11
#define LB_TOUCH_CLK_PORT GPIOB
#define LB_TOUCH_CLK_PIN GPIO_PIN_1

/* Fixed AFIO: TIM3_CH2 on PA7 and USART1 use the board's current routing. */
#define LB_STEP_TIMER TIM3
#define LB_STEP_CHANNEL TIM_CHANNEL_2
#define LB_STEP_PORT GPIOA
#define LB_STEP_PIN GPIO_PIN_7
#define LB_DEBUG_UART USART1
#define LB_UART_TX_PORT GPIOA
#define LB_UART_TX_PIN GPIO_PIN_9
#define LB_UART_RX_PORT GPIOA
#define LB_UART_RX_PIN GPIO_PIN_10

/* Fixed FSMC bus for the ATK-MD0280 LCD connector. */
#define LB_LCD_RS_PORT GPIOG
#define LB_LCD_RS_PIN GPIO_PIN_0
#define LB_LCD_CS_PORT GPIOG
#define LB_LCD_CS_PIN GPIO_PIN_12
#define LB_LCD_RD_PORT GPIOD
#define LB_LCD_RD_PIN GPIO_PIN_4
#define LB_LCD_WR_PORT GPIOD
#define LB_LCD_WR_PIN GPIO_PIN_5
#define LB_LCD_D0_PORT GPIOD
#define LB_LCD_D0_PIN GPIO_PIN_14
#define LB_LCD_D1_PORT GPIOD
#define LB_LCD_D1_PIN GPIO_PIN_15
#define LB_LCD_D2_PORT GPIOD
#define LB_LCD_D2_PIN GPIO_PIN_0
#define LB_LCD_D3_PORT GPIOD
#define LB_LCD_D3_PIN GPIO_PIN_1
#define LB_LCD_D4_PORT GPIOE
#define LB_LCD_D4_PIN GPIO_PIN_7
#define LB_LCD_D5_PORT GPIOE
#define LB_LCD_D5_PIN GPIO_PIN_8
#define LB_LCD_D6_PORT GPIOE
#define LB_LCD_D6_PIN GPIO_PIN_9
#define LB_LCD_D7_PORT GPIOE
#define LB_LCD_D7_PIN GPIO_PIN_10
#define LB_LCD_D8_PORT GPIOE
#define LB_LCD_D8_PIN GPIO_PIN_11
#define LB_LCD_D9_PORT GPIOE
#define LB_LCD_D9_PIN GPIO_PIN_12
#define LB_LCD_D10_PORT GPIOE
#define LB_LCD_D10_PIN GPIO_PIN_13
#define LB_LCD_D11_PORT GPIOE
#define LB_LCD_D11_PIN GPIO_PIN_14
#define LB_LCD_D12_PORT GPIOE
#define LB_LCD_D12_PIN GPIO_PIN_15
#define LB_LCD_D13_PORT GPIOD
#define LB_LCD_D13_PIN GPIO_PIN_8
#define LB_LCD_D14_PORT GPIOD
#define LB_LCD_D14_PIN GPIO_PIN_9
#define LB_LCD_D15_PORT GPIOD
#define LB_LCD_D15_PIN GPIO_PIN_10
#endif
