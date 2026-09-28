/* Host check for landscape drawing bounds and touch targets. */
#include "ui.h"
#include "BSP/ATK_MD0280/atk_md0280.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned credit_seen;

void atk_md0280_clear(uint16_t color) { (void)color; }

void atk_md0280_fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
    (void)color;
    assert(x1<=x2 && x2<320);
    assert(y1<=y2 && y2<240);
}

void atk_md0280_show_string(uint16_t x,uint16_t y,uint16_t width,uint16_t height,
                            char *str,atk_md0280_lcd_font_t font,uint16_t color)
{
    (void)font; (void)color;
    assert(x<320 && y<240);
    assert(x+width<=320 && y+height<=240);
    assert(strcmp(str,"LINEAR MOTION")!=0);
    if (strcmp(str,"Ver2.0 by ZJU Atomic Lab")==0) credit_seen++;
}

static void hit(const Control *s,uint16_t x,uint16_t y,uint8_t key)
{
    assert(UI_Hit(s,x,y)==key);
}

int main(void)
{
    Control s={0};
    s.ready=true;
    s.set_speed=1200;
    UI_Draw(&s,true);
    assert(credit_seen==1);
    hit(&s,300,46,K_HASH);
    hit(&s,43,142,K_F1);
    hit(&s,121,142,K_ESC);
    hit(&s,199,142,K_LEFT);
    hit(&s,277,142,K_RIGHT);
    hit(&s,82,189,K_DOWN);
    hit(&s,238,189,K_UP);
    hit(&s,160,142,K_NONE);
    hit(&s,319,239,K_NONE);

    s.editing=true;
    s.input=1200;
    UI_Draw(&s,false);
    assert(credit_seen==1);
    const uint8_t keys[]={K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,K_STAR,K_0,K_HASH};
    for (unsigned row=0;row<4;row++) {
        for (unsigned col=0;col<3;col++) {
            hit(&s,(uint16_t)(160+60*col),(uint16_t)(53+50*row),keys[row*3+col]);
        }
    }
    hit(&s,66,153,K_ESC);
    hit(&s,66,203,K_ENTER);
    hit(&s,188,53,K_NONE);
    hit(&s,319,239,K_NONE);
    puts("landscape UI bounds and hit targets: ok");
    return 0;
}
