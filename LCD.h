#ifndef LCD_H_
#define LCD_H_

#include "I2C.h"

#define LCD_I2C_ADDRESS 0x27

void LCD_Init(void);               
void LCD_Command(uint8_t cmd);     
void LCD_Char(char data);          
void LCD_String(char *str);        
void LCD_SetCursor(uint8_t row, uint8_t col); 
void LCD_Clear(void);              

#endif
