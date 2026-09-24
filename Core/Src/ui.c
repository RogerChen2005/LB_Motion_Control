/* File: ui.c
 * Purpose: Draws the motion screen and speed keypad with the official ATK-MD0280 LCD driver.
 */
#include "ui.h"
#include "BSP/ATK_MD0280/atk_md0280.h"
#include <stdio.h>
#include <string.h>

#define BG 0x0863
#define PANEL 0x1926
#define WHITE 0xFFFF
#define MUTED 0x9CD3
#define CYAN 0x36BB
#define GREEN 0x252D
#define RED 0xC986
static void text(int x,int y,const char *s,atk_md0280_lcd_font_t font,uint16_t color)
{ atk_md0280_show_string(x,y,240-x,320-y,(char *)s,font,color); }
static void box(int x,int y,int w,int h,uint16_t color)
{ atk_md0280_fill(x,y,x+w-1,y+h-1,color); }
static void button(int x,int y,int w,int h,const char *label,uint16_t color)
{
    box(x,y,w,h,color);
    text(x+(w-(int)strlen(label)*8)/2,y+(h-16)/2,label,ATK_MD0280_LCD_FONT_16,WHITE);
}
void UI_Draw(const Control *s,bool force)
{
    static bool initialized, editing;
    static uint32_t old_input, old_target, old_speed;
    static unsigned old_state;
    unsigned state=(unsigned)s->reverse|((unsigned)s->run<<1)|((unsigned)s->jog<<2)|
                   ((unsigned)s->locked<<3)|((unsigned)s->input_error<<4)|((unsigned)s->ready<<5)|
                   ((unsigned)(s->speed_milli!=0)<<6);
    bool full=force||!initialized||editing!=s->editing;
    if (!full && old_input==s->input && old_target==s->set_speed && old_speed==s->speed_milli && old_state==state) return;
    if (full) {
        atk_md0280_clear(BG); box(0,0,240,26,PANEL);
        text(10,5,s->editing ? "SET SPEED" : "LINEAR MOTION",ATK_MD0280_LCD_FONT_16,WHITE);
    }
    char b[40];
    if (s->editing) {
        box(8,32,224,48,PANEL);
        snprintf(b,sizeof(b),"%lu",(unsigned long)s->input);
        text(16,36,b,ATK_MD0280_LCD_FONT_24,s->input_error ? RED : CYAN);
        snprintf(b,sizeof(b),"0-%u mm/min",MAX_SPEED_MM_MIN);
        text(16,64,b,ATK_MD0280_LCD_FONT_12,s->input_error ? 0xFBE0 : MUTED);
        if (full) {
            const char *labels[]={"1","2","3","4","5","6","7","8","9","DEL","0","CLR"};
            for (int i=0;i<12;i++) button(8+(i%3)*76,88+(i/3)*44,72,38,labels[i],PANEL);
            button(8,270,108,42,"CANCEL",PANEL); button(124,270,108,42,"SAVE",GREEN);
        }
    } else {
        if (full || old_target!=s->set_speed) {
            box(8,32,224,54,PANEL); text(16,36,"SET SPEED / mm/min",ATK_MD0280_LCD_FONT_12,MUTED);
            snprintf(b,sizeof(b),"%lu",(unsigned long)s->set_speed);
            text(16,52,b,ATK_MD0280_LCD_FONT_32,CYAN); text(190,62,"EDIT",ATK_MD0280_LCD_FONT_12,MUTED);
        }
        box(8,94,224,18,BG);
        snprintf(b,sizeof(b),"CMD %lu.%lu mm/min",(unsigned long)(s->speed_milli/1000),(unsigned long)(s->speed_milli%1000/100));
        text(12,94,b,ATK_MD0280_LCD_FONT_16,WHITE);
        if (full || old_state!=state) {
            box(8,116,224,18,BG);
            const char *status=!s->ready ? "INHIBITED" : s->locked ? "RELEASE ALL KEYS" : s->jog ? "JOG / HOLD" : s->run ? "RUNNING" : s->speed_milli ? "DECELERATING" : "STOPPED";
            text(12,116,status,ATK_MD0280_LCD_FONT_16,MUTED);
            button(8,142,108,42,s->run ? "PAUSE" : "START",GREEN); button(124,142,108,42,"STOP",RED);
            button(8,192,108,36,"REVERSE",s->reverse ? 0x2B16 : PANEL);
            button(124,192,108,36,"FORWARD",s->reverse ? PANEL : 0x2B16);
        }
        if (full) {
            button(8,236,108,42,"- JOG",0x2250); button(124,236,108,42,"+ JOG",0x2250);
            snprintf(b,sizeof(b),"LEAD 4mm / %u P-REV",PULSES_PER_REV);
            text(10,288,b,ATK_MD0280_LCD_FONT_12,MUTED);
            text(10,304,"F1 RUN  ENT SAVE  ESC STOP",ATK_MD0280_LCD_FONT_12,MUTED);
        }
    }
    initialized=true; editing=s->editing; old_input=s->input; old_target=s->set_speed;
    old_speed=s->speed_milli; old_state=state;
}
uint8_t UI_Hit(const Control *s,uint16_t x,uint16_t y)
{
    if (x>=240 || y>=320) return K_NONE;
    if (s->editing) {
        static const uint8_t keys[]={K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,K_STAR,K_0,K_HASH};
        if (x>=8 && x<232 && y>=88 && y<258) {
            unsigned col=(x-8)/76,row=(y-88)/44;
            if (col<3 && row<4 && (x-8)%76<72 && (y-88)%44<38) return keys[row*3+col];
        }
        if (y>=270 && y<312) { if (x>=8 && x<116) return K_ESC; if (x>=124 && x<232) return K_ENTER; }
    } else {
        if (x>=8 && x<232 && y>=32 && y<86) return K_HASH;
        if (y>=142 && y<184) { if (x>=8 && x<116) return K_F1; if (x>=124 && x<232) return K_ESC; }
        if (y>=192 && y<228) { if (x>=8 && x<116) return K_LEFT; if (x>=124 && x<232) return K_RIGHT; }
        if (y>=236 && y<278) { if (x>=8 && x<116) return K_DOWN; if (x>=124 && x<232) return K_UP; }
    }
    return K_NONE;
}
