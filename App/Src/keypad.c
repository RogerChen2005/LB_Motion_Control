/* File: keypad.c
 * Purpose: Drive one open-drain row at a time and sample four pulled-up columns.
 */
#include "keypad.h"
#include "board_pins.h"

static const BoardPin rows[5]={
    {LB_KEY_ROW0_PORT,LB_KEY_ROW0_PIN}, {LB_KEY_ROW1_PORT,LB_KEY_ROW1_PIN},
    {LB_KEY_ROW2_PORT,LB_KEY_ROW2_PIN}, {LB_KEY_ROW3_PORT,LB_KEY_ROW3_PIN},
    {LB_KEY_ROW4_PORT,LB_KEY_ROW4_PIN}
};
static const BoardPin cols[4]={
    {LB_KEY_COL0_PORT,LB_KEY_COL0_PIN}, {LB_KEY_COL1_PORT,LB_KEY_COL1_PIN},
    {LB_KEY_COL2_PORT,LB_KEY_COL2_PIN}, {LB_KEY_COL3_PORT,LB_KEY_COL3_PIN}
};
static uint8_t active_row;
static uint32_t scan_mask;

void Keypad_Init(void)
{
    GPIO_InitTypeDef gpio={0};
    for (unsigned i=0;i<5;i++) {
        Board_EnableGpioClock(rows[i].port);
        HAL_GPIO_WritePin(rows[i].port,rows[i].pin,GPIO_PIN_SET);
        gpio.Pin=rows[i].pin; gpio.Mode=GPIO_MODE_OUTPUT_OD;
        gpio.Speed=GPIO_SPEED_FREQ_LOW; HAL_GPIO_Init(rows[i].port,&gpio);
    }
    for (unsigned i=0;i<4;i++) {
        Board_EnableGpioClock(cols[i].port);
        gpio.Pin=cols[i].pin; gpio.Mode=GPIO_MODE_INPUT; gpio.Pull=GPIO_PULLUP;
        HAL_GPIO_Init(cols[i].port,&gpio);
    }
    active_row=0; scan_mask=0;
    HAL_GPIO_WritePin(rows[0].port,rows[0].pin,GPIO_PIN_RESET);
}
bool Keypad_Scan1ms(uint32_t *mask)
{
    for (unsigned col=0;col<4;col++) {
        if ((cols[col].port->IDR & cols[col].pin)==0)
            scan_mask|=1UL<<(active_row*4U+col);
    }
    HAL_GPIO_WritePin(rows[active_row].port,rows[active_row].pin,GPIO_PIN_SET);
    if (++active_row==5) {
        active_row=0;
        *mask=scan_mask; scan_mask=0;
        HAL_GPIO_WritePin(rows[0].port,rows[0].pin,GPIO_PIN_RESET);
        return true;
    }
    HAL_GPIO_WritePin(rows[active_row].port,rows[active_row].pin,GPIO_PIN_RESET);
    return false;
}
