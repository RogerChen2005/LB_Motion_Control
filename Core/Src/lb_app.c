/* File: lb_app.c
 * Purpose: Connects the keyboard, touch panel, serial diagnostics, and TIM1 pulse output.
 */
#include "lb_app.h"
#include "main.h"
#include "control.h"
#include "ui.h"
#include "BSP/ATK_MD0280/atk_md0280.h"
#include "BSP/ATK_MD0280/atk_md0280_fsmc.h"
#include "SYSTEM/delay/delay.h"
#include <stdio.h>
#include <string.h>

static TIM_HandleTypeDef pulse_timer;
static UART_HandleTypeDef debug_uart;
static Control control;
static volatile bool started;
static uint8_t scan_row, tick_div;
static uint32_t scan_mask;
static bool pulse_running;
static uint32_t previous_frequency;
static volatile uint32_t touch_mask, touch_time, heartbeat;
volatile uint16_t g_lcd_id;
static const uint16_t rows[5]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4};
static const uint16_t cols[4]={GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8};

/* Pure timing calculation, also exercised by host tests. */
#include "pulse_math.h"

static void motor_init(void)
{
    GPIO_InitTypeDef g={0};
    __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_TIM1_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7|GPIO_PIN_8,GPIO_PIN_RESET);
    g.Pin=GPIO_PIN_7; g.Mode=GPIO_MODE_OUTPUT_PP; g.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&g);
    g.Pin=GPIO_PIN_8; g.Mode=GPIO_MODE_AF_PP; HAL_GPIO_Init(GPIOA,&g);
    pulse_timer.Instance=TIM1;
    pulse_timer.Init.Prescaler=47; pulse_timer.Init.CounterMode=TIM_COUNTERMODE_UP;
    pulse_timer.Init.Period=999; pulse_timer.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    pulse_timer.Init.RepetitionCounter=0; pulse_timer.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&pulse_timer)!=HAL_OK) Error_Handler();
    TIM_OC_InitTypeDef oc={0};
    oc.OCMode=TIM_OCMODE_PWM1; oc.Pulse=500; oc.OCPolarity=TIM_OCPOLARITY_HIGH;
    oc.OCNPolarity=TIM_OCNPOLARITY_HIGH; oc.OCFastMode=TIM_OCFAST_DISABLE;
    oc.OCIdleState=TIM_OCIDLESTATE_RESET; oc.OCNIdleState=TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&pulse_timer,&oc,TIM_CHANNEL_1)!=HAL_OK) Error_Handler();
    SET_BIT(TIM1->BDTR,TIM_BDTR_OSSI|TIM_BDTR_OSSR);
    /* Freeze the pulse generator when halted through SWD. */
    __HAL_DBGMCU_FREEZE_TIM1();
}
void LB_MotorOff(void)
{
    if (pulse_timer.Instance==TIM1) {
        CLEAR_BIT(TIM1->BDTR,TIM_BDTR_MOE);
        CLEAR_BIT(TIM1->CR1,TIM_CR1_CEN);
        CLEAR_BIT(TIM1->CCER,TIM_CCER_CC1E);
        /* Keep HAL channel state reusable after the next START command. */
        (void)HAL_TIM_PWM_Stop(&pulse_timer,TIM_CHANNEL_1);
    }
    pulse_running=false; previous_frequency=0;
}
static void motor_apply(void)
{
    uint32_t frequency=Control_FrequencyMilliHz(&control);
    if (!frequency) {
        if (pulse_running) LB_MotorOff();
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,control.output_reverse ? GPIO_PIN_SET : GPIO_PIN_RESET);
        return;
    }
    if (frequency==previous_frequency) return;
    uint32_t timer_clock=HAL_RCC_GetPCLK2Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE2)!=RCC_CFGR_PPRE2_DIV1) timer_clock*=2;
    uint16_t prescaler, period;
    Pulse_Calculate(timer_clock,frequency,&prescaler,&period);
    /* PSC, ARR and CCR all transfer together on the next natural overflow. */
    SET_BIT(TIM1->CR1,TIM_CR1_UDIS);
    TIM1->PSC=prescaler; TIM1->ARR=period; TIM1->CCR1=((uint32_t)period+1U)/2U;
    CLEAR_BIT(TIM1->CR1,TIM_CR1_UDIS);
    if (!pulse_running) {
        TIM1->EGR=TIM_EGR_UG; TIM1->SR=0; TIM1->CNT=0;
        if (HAL_TIM_PWM_Start(&pulse_timer,TIM_CHANNEL_1)!=HAL_OK) Error_Handler();
        pulse_running=true;
    }
    previous_frequency=frequency;
}
static void keypad_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef g={0};
    HAL_GPIO_WritePin(GPIOC,0x001F,GPIO_PIN_SET);
    g.Pin=0x001F; g.Mode=GPIO_MODE_OUTPUT_OD; g.Speed=GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC,&g); /* Inactive rows float: no output contention. */
    g.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
    g.Mode=GPIO_MODE_INPUT; g.Pull=GPIO_PULLUP; HAL_GPIO_Init(GPIOC,&g);
    HAL_GPIO_WritePin(GPIOC,rows[0],GPIO_PIN_RESET);
}
static void serial_init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    GPIO_InitTypeDef g={0}; g.Pin=GPIO_PIN_9; g.Mode=GPIO_MODE_AF_PP;
    g.Speed=GPIO_SPEED_FREQ_HIGH; HAL_GPIO_Init(GPIOA,&g);
    g.Pin=GPIO_PIN_10; g.Mode=GPIO_MODE_INPUT; g.Pull=GPIO_PULLUP; HAL_GPIO_Init(GPIOA,&g);
    debug_uart.Instance=USART1;
    debug_uart.Init.BaudRate=115200; debug_uart.Init.WordLength=UART_WORDLENGTH_8B;
    debug_uart.Init.StopBits=UART_STOPBITS_1; debug_uart.Init.Parity=UART_PARITY_NONE;
    debug_uart.Init.Mode=UART_MODE_TX_RX; debug_uart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    debug_uart.Init.OverSampling=UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&debug_uart)!=HAL_OK) Error_Handler();
}
void LB_Tick1ms(void)
{
    if (!started) return;
    /* Read a row which has had a full 1 ms to settle. */
    uint32_t levels=GPIOC->IDR;
    for (uint8_t c=0;c<4;c++) if (!(levels&cols[c])) scan_mask|=1UL<<(scan_row*4U+c);
    HAL_GPIO_WritePin(GPIOC,rows[scan_row],GPIO_PIN_SET);
    if (++scan_row==5) {
        uint32_t tm=(uint32_t)(HAL_GetTick()-touch_time)<80U ? touch_mask : 0;
        /* Two input devices pressed together are treated as a multi-key fault,
           even when both happen to represent the same logical button. */
        uint32_t merged=scan_mask && tm ? 3U : scan_mask|tm;
        scan_row=0; Control_Keys(&control,merged); scan_mask=0;
    }
    HAL_GPIO_WritePin(GPIOC,rows[scan_row],GPIO_PIN_RESET);
    if (++tick_div==10) {
        tick_div=0;
        if ((uint32_t)(HAL_GetTick()-heartbeat)>250U) control.ready=false;
        Control_Tick10ms(&control); motor_apply();
    }
}
static void app_clock(void)
{
    /* Elite F103 board: 8 MHz HSE, 72 MHz CPU and TIM1, APB1 = 36 MHz. */
    RCC_OscInitTypeDef o={0}; RCC_ClkInitTypeDef c={0};
    o.OscillatorType=RCC_OSCILLATORTYPE_HSE; o.HSEState=RCC_HSE_ON;
    o.PLL.PLLState=RCC_PLL_ON; o.PLL.PLLSource=RCC_PLLSOURCE_HSE; o.PLL.PLLMUL=RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&o)!=HAL_OK) Error_Handler();
    c.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    c.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; c.AHBCLKDivider=RCC_SYSCLK_DIV1;
    c.APB1CLKDivider=RCC_HCLK_DIV2; c.APB2CLKDivider=RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&c,FLASH_LATENCY_2)!=HAL_OK) Error_Handler();
}
void LB_Init(void)
{
    app_clock(); delay_init(72);
    motor_init(); keypad_init(); serial_init();
    /* Official driver includes interactive five-point calibration at boot.
       PWM remains disabled throughout calibration. */
    bool panel=atk_md0280_init()==ATK_MD0280_EOK;
    atk_md0280_fsmc_write_cmd(0xD3);
    (void)atk_md0280_fsmc_read_dat(); (void)atk_md0280_fsmc_read_dat();
    g_lcd_id=(uint16_t)(atk_md0280_fsmc_read_dat()<<8);
    g_lcd_id|=atk_md0280_fsmc_read_dat()&0xFF;
    if (g_lcd_id!=0x9341) {
        atk_md0280_fsmc_write_cmd(0x04);
        (void)atk_md0280_fsmc_read_dat(); (void)atk_md0280_fsmc_read_dat();
        g_lcd_id=(uint16_t)(atk_md0280_fsmc_read_dat()<<8);
        g_lcd_id|=atk_md0280_fsmc_read_dat()&0xFF;
    }
    Control_Init(&control,panel);
    char message[100];
    int n=snprintf(message,sizeof(message),"LB HAL: LCD=%04X, %s; PPR=%u; lead=4mm\r\n",
        g_lcd_id,panel ? "READY" : "UNSUPPORTED - MOTION BLOCKED",(unsigned)PULSES_PER_REV);
    HAL_UART_Transmit(&debug_uart,(uint8_t *)message,(uint16_t)n,100);
    if (panel) UI_Draw(&control,true);
    /* No held key at boot may start motion: require a clean release first. */
    control.locked=true; heartbeat=HAL_GetTick(); started=true;
}
void LB_Poll(void)
{
    static uint32_t previous, touch_previous;
    static bool pen_held, touch_editing;
    static uint8_t anchor=K_NONE;
    uint32_t now=HAL_GetTick();
    heartbeat=now;
    Control s;
    uint32_t primask=__get_PRIMASK(); __disable_irq(); s=control; __set_PRIMASK(primask);
    if (g_lcd_id!=0x9341 && g_lcd_id!=0x8552) return;
    if ((uint32_t)(now-touch_previous)>=10U) {
        touch_previous=now;
        uint16_t x=0,y=0;
        uint8_t result=atk_md0280_touch_scan(&x,&y);
        uint8_t key=result==ATK_MD0280_TOUCH_EOK ? UI_Hit(&s,x,y) : K_NONE;
        bool down=ATK_MD0280_TOUCH_READ_PEN()==0;
        if (!down) { pen_held=false; anchor=K_NONE; }
        else if (!pen_held) { pen_held=true; anchor=key; touch_editing=s.editing; }
        else if (key!=anchor || touch_editing!=s.editing) anchor=K_NONE;
        uint32_t mask=down && anchor!=K_NONE ? 1UL<<anchor : 0;
        primask=__get_PRIMASK(); __disable_irq();
        touch_mask=mask; touch_time=HAL_GetTick(); __set_PRIMASK(primask);
    }
    if ((uint32_t)(now-previous)>=100U) { previous=now; UI_Draw(&s,false); }
}
