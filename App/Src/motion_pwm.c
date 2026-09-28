/* File: motion_pwm.c
 * Purpose: Generate PUL with TIM3_CH2 and change PWM frequency during acceleration.
 */
#include "motion_pwm.h"
#include "board_pins.h"
#include "pulse_math.h"
#include "main.h"

static TIM_HandleTypeDef timer;
static bool running;
static uint32_t previous_frequency;

void Motion_Init(void)
{
    GPIO_InitTypeDef gpio={0};
    Board_EnableGpioClock(LB_DIR_PORT);
    Board_EnableGpioClock(LB_ENABLE_PORT);
    Board_EnableGpioClock(LB_STEP_PORT);
    __HAL_RCC_TIM3_CLK_ENABLE();
    HAL_GPIO_WritePin(LB_DIR_PORT,LB_DIR_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LB_ENABLE_PORT,LB_ENABLE_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LB_STEP_PORT,LB_STEP_PIN,GPIO_PIN_RESET);
    gpio.Pin=LB_DIR_PIN; gpio.Mode=GPIO_MODE_OUTPUT_PP; gpio.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LB_DIR_PORT,&gpio);
    gpio.Pin=LB_ENABLE_PIN;
    HAL_GPIO_Init(LB_ENABLE_PORT,&gpio);
    gpio.Pin=LB_STEP_PIN; gpio.Mode=GPIO_MODE_AF_PP;
    HAL_GPIO_Init(LB_STEP_PORT,&gpio);
    timer.Instance=LB_STEP_TIMER;
    timer.Init.Prescaler=47; timer.Init.CounterMode=TIM_COUNTERMODE_UP;
    timer.Init.Period=999; timer.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    timer.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&timer)!=HAL_OK) Error_Handler();
    TIM_OC_InitTypeDef oc={0};
    oc.OCMode=TIM_OCMODE_PWM1; oc.Pulse=500; oc.OCPolarity=TIM_OCPOLARITY_HIGH;
    oc.OCFastMode=TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&timer,&oc,LB_STEP_CHANNEL)!=HAL_OK) Error_Handler();
    __HAL_DBGMCU_FREEZE_TIM3();
    /* ENA+ is active high; retain holding torque while stationary. */
    HAL_GPIO_WritePin(LB_ENABLE_PORT,LB_ENABLE_PIN,GPIO_PIN_SET);
}

void Motion_EmergencyOff(void)
{
    if (timer.Instance==LB_STEP_TIMER) {
        CLEAR_BIT(TIM3->CR1,TIM_CR1_CEN);
        CLEAR_BIT(TIM3->CCER,TIM_CCER_CC2E);
        HAL_GPIO_WritePin(LB_ENABLE_PORT,LB_ENABLE_PIN,GPIO_PIN_RESET);
    }
    running=false; previous_frequency=0;
}

static void stop_pwm(void)
{
    CLEAR_BIT(TIM3->CR1,TIM_CR1_CEN);
    CLEAR_BIT(TIM3->CCER,TIM_CCER_CC2E);
    (void)HAL_TIM_PWM_Stop(&timer,LB_STEP_CHANNEL); /* Restore HAL channel state. */
    running=false; previous_frequency=0;
}

void Motion_Apply(bool reverse,uint32_t frequency_millihz)
{
    if (!frequency_millihz) {
        if (running) stop_pwm();
        HAL_GPIO_WritePin(LB_DIR_PORT,LB_DIR_PIN,reverse ? GPIO_PIN_SET : GPIO_PIN_RESET);
        return;
    }
    if (frequency_millihz==previous_frequency) return;
    uint32_t clock=HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1)!=RCC_CFGR_PPRE1_DIV1) clock*=2;
    uint16_t prescaler, period;
    Pulse_Calculate(clock,frequency_millihz,&prescaler,&period);
    SET_BIT(TIM3->CR1,TIM_CR1_UDIS);
    TIM3->PSC=prescaler; TIM3->ARR=period; TIM3->CCR2=((uint32_t)period+1U)/2U;
    CLEAR_BIT(TIM3->CR1,TIM_CR1_UDIS);
    if (!running) {
        TIM3->EGR=TIM_EGR_UG; TIM3->SR=0; TIM3->CNT=0;
        if (HAL_TIM_PWM_Start(&timer,LB_STEP_CHANNEL)!=HAL_OK) Error_Handler();
        running=true;
    }
    previous_frequency=frequency_millihz;
}
