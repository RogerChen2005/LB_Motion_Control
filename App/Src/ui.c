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
#define UI_WIDTH 320
#define UI_HEIGHT 240
static void text(int x,int y,const char *s,atk_md0280_lcd_font_t font,uint16_t color)
{ atk_md0280_show_string(x,y,UI_WIDTH-x,UI_HEIGHT-y,(char *)s,font,color); }
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
        atk_md0280_clear(BG);
        if (s->editing) {
            box(0,0,UI_WIDTH,26,PANEL);
            text(10,5,"SET SPEED",ATK_MD0280_LCD_FONT_16,WHITE);
        }
    }
    char b[40];
    if (s->editing) {
        box(8,32,116,84,PANEL);
        text(16,40,"mm/min",ATK_MD0280_LCD_FONT_12,MUTED);
        snprintf(b,sizeof(b),"%lu",(unsigned long)s->input);
        text(16,62,b,ATK_MD0280_LCD_FONT_24,s->input_error ? RED : CYAN);
        snprintf(b,sizeof(b),"0-%u mm/min",MAX_SPEED_MM_MIN);
        text(16,96,b,ATK_MD0280_LCD_FONT_12,s->input_error ? 0xFBE0 : MUTED);
        if (full) {
            const char *labels[]={"1","2","3","4","5","6","7","8","9","DEL","0","CLR"};
            for (int i=0;i<12;i++) button(132+(i%3)*60,32+(i/3)*50,56,42,labels[i],PANEL);
            button(8,132,116,42,"CANCEL",PANEL); button(8,182,116,42,"SAVE",GREEN);
        }
    } else {
        if (full || old_target!=s->set_speed) {
            box(8,8,304,66,PANEL); text(16,14,"SET SPEED / mm/min",ATK_MD0280_LCD_FONT_12,MUTED);
            snprintf(b,sizeof(b),"%lu",(unsigned long)s->set_speed);
            text(16,31,b,ATK_MD0280_LCD_FONT_32,CYAN); text(270,46,"EDIT",ATK_MD0280_LCD_FONT_12,MUTED);
        }
        box(8,79,304,18,BG);
        snprintf(b,sizeof(b),"CMD %lu.%lu mm/min",(unsigned long)(s->speed_milli/1000),(unsigned long)(s->speed_milli%1000/100));
        text(12,79,b,ATK_MD0280_LCD_FONT_16,WHITE);
        if (full || old_state!=state) {
            box(8,99,304,18,BG);
            const char *status=!s->ready ? "INHIBITED" : s->locked ? "RELEASE ALL KEYS" : s->jog ? "JOG / HOLD" : s->run ? "RUNNING" : s->speed_milli ? "DECELERATING" : "STOPPED";
            text(12,99,status,ATK_MD0280_LCD_FONT_16,MUTED);
            button(8,122,70,40,s->run ? "PAUSE" : "START",GREEN);
            button(86,122,70,40,"STOP",RED);
            button(164,122,70,40,"REVERSE",s->reverse ? 0x2B16 : PANEL);
            button(242,122,70,40,"FORWARD",s->reverse ? PANEL : 0x2B16);
        }
        if (full) {
            button(8,170,148,38,"- JOG",0x2250); button(164,170,148,38,"+ JOG",0x2250);
            const char *credit="Ver2.0 by ZJU Atomic Lab";
            text((UI_WIDTH-(int)strlen(credit)*6)/2,219,credit,ATK_MD0280_LCD_FONT_12,MUTED);
        }
    }
    initialized=true; editing=s->editing; old_input=s->input; old_target=s->set_speed;
    old_speed=s->speed_milli; old_state=state;
}
uint8_t UI_Hit(const Control *s,uint16_t x,uint16_t y)
{
    if (x>=UI_WIDTH || y>=UI_HEIGHT) return K_NONE;
    if (s->editing) {
        static const uint8_t keys[]={K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,K_STAR,K_0,K_HASH};
        if (x>=132 && x<308 && y>=32 && y<224) {
            unsigned col=(x-132)/60,row=(y-32)/50;
            if (col<3 && row<4 && (x-132)%60<56 && (y-32)%50<42) return keys[row*3+col];
        }
        if (x>=8 && x<124) { if (y>=132 && y<174) return K_ESC; if (y>=182 && y<224) return K_ENTER; }
    } else {
        if (x>=8 && x<312 && y>=8 && y<74) return K_HASH;
        if (y>=122 && y<162) {
            if (x>=8 && x<78) return K_F1;
            if (x>=86 && x<156) return K_ESC;
            if (x>=164 && x<234) return K_LEFT;
            if (x>=242 && x<312) return K_RIGHT;
        }
        if (y>=170 && y<208) { if (x>=8 && x<156) return K_DOWN; if (x>=164 && x<312) return K_UP; }
    }
    return K_NONE;
}
