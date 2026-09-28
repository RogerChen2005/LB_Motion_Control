/**
 ****************************************************************************************************
 * @file        atk_md0280_fsmc.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0280模块FSMC接口驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MD0280_FSMC_H
#define __ATK_MD0280_FSMC_H

#include "./SYSTEM/sys/sys.h"
#include "board_pins.h"

/* ATK-MD0280模块FSMC接口定义 */
#define ATK_MD0280_FSMC_BANK                    FSMC_NORSRAM_BANK4                          /* ATK-MD0280模块所接FSMC的Bank */
#define ATK_MD0280_FSMC_BANK_ADDR               (0x6C000000)
#define ATK_MD0280_FSMC_REG_SEL                 (10)
#define ATK_MD0280_FSMC_READ_AST                0x00                                        /* 读时序的地址建立时间，单位：HCLK */
#define ATK_MD0280_FSMC_READ_DST                0x0F                                        /* 读时序的数据建立时间，单位：HCLK */
#define ATK_MD0280_FSMC_WRITE_AST               0x00                                        /* 写时序的地址建立时间，单位：HCLK */
#define ATK_MD0280_FSMC_WRITE_DST               0x01                                        /* 写时序的数据建立时间，单位：HCLK */
#define ATK_MD0280_FSMC_CLK_ENABLE()            do{ __HAL_RCC_FSMC_CLK_ENABLE(); }while(0)  /* ATK-MD0280模块所接FSMC时钟使能 */

/* ATK-MD0280模块FSMC接口读写命令、数据地址 */
#define ATK_MD0280_FSMC_CMD_ADDR                (ATK_MD0280_FSMC_BANK_ADDR | (((1U << ATK_MD0280_FSMC_REG_SEL) - 1) << 1))
#define ATK_MD0280_FSMC_DAT_ADDR                (ATK_MD0280_FSMC_BANK_ADDR | ((1U << ATK_MD0280_FSMC_REG_SEL) << 1))

/* ATK-MD0280模块FSMC接口读写命令、数据 */
#define ATK_MD0280_FSMC_CMD_REG                 (*(volatile uint16_t *)ATK_MD0280_FSMC_CMD_ADDR)
#define ATK_MD0280_FSMC_DAT_REG                 (*(volatile uint16_t *)ATK_MD0280_FSMC_DAT_ADDR)

/* 引脚定义 */
#define ATK_MD0280_FSMC_RS_GPIO_PORT            LB_LCD_RS_PORT
#define ATK_MD0280_FSMC_RS_GPIO_PIN             LB_LCD_RS_PIN
#define ATK_MD0280_FSMC_RS_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_RS_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_CS_GPIO_PORT            LB_LCD_CS_PORT
#define ATK_MD0280_FSMC_CS_GPIO_PIN             LB_LCD_CS_PIN
#define ATK_MD0280_FSMC_CS_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_CS_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_RD_GPIO_PORT            LB_LCD_RD_PORT
#define ATK_MD0280_FSMC_RD_GPIO_PIN             LB_LCD_RD_PIN
#define ATK_MD0280_FSMC_RD_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_RD_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_WR_GPIO_PORT            LB_LCD_WR_PORT
#define ATK_MD0280_FSMC_WR_GPIO_PIN             LB_LCD_WR_PIN
#define ATK_MD0280_FSMC_WR_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_WR_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D0_GPIO_PORT            LB_LCD_D0_PORT
#define ATK_MD0280_FSMC_D0_GPIO_PIN             LB_LCD_D0_PIN
#define ATK_MD0280_FSMC_D0_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D0_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D1_GPIO_PORT            LB_LCD_D1_PORT
#define ATK_MD0280_FSMC_D1_GPIO_PIN             LB_LCD_D1_PIN
#define ATK_MD0280_FSMC_D1_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D1_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D2_GPIO_PORT            LB_LCD_D2_PORT
#define ATK_MD0280_FSMC_D2_GPIO_PIN             LB_LCD_D2_PIN
#define ATK_MD0280_FSMC_D2_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D2_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D3_GPIO_PORT            LB_LCD_D3_PORT
#define ATK_MD0280_FSMC_D3_GPIO_PIN             LB_LCD_D3_PIN
#define ATK_MD0280_FSMC_D3_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D3_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D4_GPIO_PORT            LB_LCD_D4_PORT
#define ATK_MD0280_FSMC_D4_GPIO_PIN             LB_LCD_D4_PIN
#define ATK_MD0280_FSMC_D4_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D4_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D5_GPIO_PORT            LB_LCD_D5_PORT
#define ATK_MD0280_FSMC_D5_GPIO_PIN             LB_LCD_D5_PIN
#define ATK_MD0280_FSMC_D5_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D5_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D6_GPIO_PORT            LB_LCD_D6_PORT
#define ATK_MD0280_FSMC_D6_GPIO_PIN             LB_LCD_D6_PIN
#define ATK_MD0280_FSMC_D6_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D6_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D7_GPIO_PORT            LB_LCD_D7_PORT
#define ATK_MD0280_FSMC_D7_GPIO_PIN             LB_LCD_D7_PIN
#define ATK_MD0280_FSMC_D7_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D7_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D8_GPIO_PORT            LB_LCD_D8_PORT
#define ATK_MD0280_FSMC_D8_GPIO_PIN             LB_LCD_D8_PIN
#define ATK_MD0280_FSMC_D8_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D8_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D9_GPIO_PORT            LB_LCD_D9_PORT
#define ATK_MD0280_FSMC_D9_GPIO_PIN             LB_LCD_D9_PIN
#define ATK_MD0280_FSMC_D9_GPIO_CLK_ENABLE()    do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D9_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D10_GPIO_PORT           LB_LCD_D10_PORT
#define ATK_MD0280_FSMC_D10_GPIO_PIN            LB_LCD_D10_PIN
#define ATK_MD0280_FSMC_D10_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D10_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D11_GPIO_PORT           LB_LCD_D11_PORT
#define ATK_MD0280_FSMC_D11_GPIO_PIN            LB_LCD_D11_PIN
#define ATK_MD0280_FSMC_D11_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D11_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D12_GPIO_PORT           LB_LCD_D12_PORT
#define ATK_MD0280_FSMC_D12_GPIO_PIN            LB_LCD_D12_PIN
#define ATK_MD0280_FSMC_D12_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D12_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D13_GPIO_PORT           LB_LCD_D13_PORT
#define ATK_MD0280_FSMC_D13_GPIO_PIN            LB_LCD_D13_PIN
#define ATK_MD0280_FSMC_D13_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D13_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D14_GPIO_PORT           LB_LCD_D14_PORT
#define ATK_MD0280_FSMC_D14_GPIO_PIN            LB_LCD_D14_PIN
#define ATK_MD0280_FSMC_D14_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D14_GPIO_PORT); }while(0)
#define ATK_MD0280_FSMC_D15_GPIO_PORT           LB_LCD_D15_PORT
#define ATK_MD0280_FSMC_D15_GPIO_PIN            LB_LCD_D15_PIN
#define ATK_MD0280_FSMC_D15_GPIO_CLK_ENABLE()   do{ Board_EnableGpioClock(ATK_MD0280_FSMC_D15_GPIO_PORT); }while(0)

/* 操作函数 */
void atk_md0280_fsmc_init(void);                                                            /* ATK-MD0280模块FSMC接口初始化 */
static inline void atk_md0280_fsmc_write_cmd(volatile uint16_t cmd)                         /* ATK-MD0280模块通过FSMC接口写命令 */
{
    ATK_MD0280_FSMC_CMD_REG = cmd;
}
static inline void atk_md0280_fsmc_write_dat(volatile uint16_t dat)                         /* ATK-MD0280模块通过FSMC接口写数据 */
{
    ATK_MD0280_FSMC_DAT_REG = dat;
}
static inline void atk_md0280_fsmc_write_reg(volatile uint16_t reg, volatile uint16_t dat)  /* ATK-MD0280模块通过FSMC接口写寄存器 */
{
    ATK_MD0280_FSMC_CMD_REG = reg;
    ATK_MD0280_FSMC_DAT_REG = dat;
}
static inline uint16_t atk_md0280_fsmc_read_dat(void)                              /* ATK-MD0280模块通过FSMC接口读数据 */
{
    uint16_t dat;
    
    __nop();
    __nop();
    dat = ATK_MD0280_FSMC_DAT_REG;
    
    return dat;
}

#endif
