/* File: display_panel.c
 * Purpose: Start the official LCD/touch BSP and read the LCD controller ID.
 */
#include "display_panel.h"
#include "BSP/ATK_MD0280/atk_md0280.h"
#include "BSP/ATK_MD0280/atk_md0280_fsmc.h"

static uint16_t display_id;

bool DisplayPanel_Init(void)
{
    bool ready=atk_md0280_init()==ATK_MD0280_EOK;
    if (!ready) return false;
    /* Touch calibration runs in portrait during BSP init; rotate afterwards. */
    if (atk_md0280_set_disp_dir(ATK_MD0280_LCD_DISP_DIR_90)!=ATK_MD0280_EOK)
        return false;
    atk_md0280_fsmc_write_cmd(0xD3);
    (void)atk_md0280_fsmc_read_dat(); (void)atk_md0280_fsmc_read_dat();
    display_id=(uint16_t)(atk_md0280_fsmc_read_dat()<<8);
    display_id|=atk_md0280_fsmc_read_dat()&0xFF;
    if (display_id!=0x9341) {
        atk_md0280_fsmc_write_cmd(0x04);
        (void)atk_md0280_fsmc_read_dat(); (void)atk_md0280_fsmc_read_dat();
        display_id=(uint16_t)(atk_md0280_fsmc_read_dat()<<8);
        display_id|=atk_md0280_fsmc_read_dat()&0xFF;
    }
    return ready;
}
uint16_t DisplayPanel_Id(void) { return display_id; }
