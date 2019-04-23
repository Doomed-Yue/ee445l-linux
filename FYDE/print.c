//
//  print.c
//  
//
//  Created by apple on 4/22/19.
//

#include "Print.h"


void LCD_OutDec(int32_t data){
    if(data < 0){
        data = -data;
        ST7735_OutChar('-');
    }
    if(data < 10){
        ST7735_OutChar(data + '0');
        return;
    }
    LCD_OutDec(data / 10);
    ST7735_OutChar(data % 10 + '0');
}


void LCD_OutFix(int32_t data){
    if(data < 0){
        ST7735_OutChar('-');
        data = -data;
    }
    uint8_t thousand = data / 1000;
    uint8_t hundred = (data - thousand * 1000) / 100;
    uint8_t tenth = (data - thousand * 1000 - hundred * 10) / 10;
    uint8_t single = data % 10;
    ST7735_OutChar(thousand + '0');
    ST7735_OutChar('.');
    ST7735_OutChar(hundred + '0');
    ST7735_OutChar(tenth + '0');
    ST7735_OutChar(single + '0');
}








