// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "shared/ST7735.h"
#include "Accelerometer.h"
#include "Print.h"

long accelX, accelY, accelZ;
uint32_t gForceX, gForceY, gForceZ;

void processData(){
	gForceX = ((int)(accelX / 16384.0) * 1000) % 1000;
	gForceY = ((int)(accelY / 16384.0) * 1000) % 1000;
	gForceZ = ((int)(accelZ / 16384.0) * 1000) % 1000;
}

void printAccel(){
	ST7735_SetCursor(0, 0);
	LCD_OutFix(gForceX);
    ST7735_OutChar(' ');
	ST7735_SetCursor(0, 1);
	LCD_OutFix(gForceY);
    ST7735_OutChar(' ');
	ST7735_SetCursor(0, 2);
	LCD_OutFix(gForceZ);
    ST7735_OutChar(' ');
}

void printAccelRaw(){
    ST7735_SetCursor(0, 0);
    LCD_OutDec(accelX);
     ST7735_OutChar(' ');
    ST7735_SetCursor(0, 5);
    LCD_OutDec(accelY);
     ST7735_OutChar(' ');
    ST7735_SetCursor(0, 10);
    LCD_OutDec(accelZ);
     ST7735_OutChar(' ');
}

int main(void){
	PLL_Init(Bus80MHz);
	I2C_Init();
	Accele_Init();
	ST7735_InitR(INITR_REDTAB);
	while(1){
		I2C_Send1(104, 0x3B);
		accelX = (I2C_Recv(104) << 8) | I2C_Recv(104);
		accelY = (I2C_Recv(104) << 8) | I2C_Recv(104);
		accelZ = (I2C_Recv(104) << 8) | I2C_Recv(104);
        printAccelRaw();
	}
	
}




