#include "LCD.h"

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_WRITE 0x01

void LCD_WriteNibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    I2C_Start();
    I2C_Write(LCD_I2C_ADDRESS << 1);
    I2C_Write(data | LCD_ENABLE);  
    I2C_Write(data & ~LCD_ENABLE); 
    I2C_Stop();
}

void LCD_Command(uint8_t cmd) {
    LCD_WriteNibble(cmd & 0xF0, 0); 
    LCD_WriteNibble(cmd << 4, 0);  
}

void LCD_Char(char data) {
    LCD_WriteNibble(data & 0xF0, LCD_WRITE); 
    LCD_WriteNibble(data << 4, LCD_WRITE); 
}

void LCD_Init(void) {
    I2C_Init();
    _delay_ms(50);

    LCD_Command(0x30); 
    _delay_ms(5);
    LCD_Command(0x30);
    _delay_us(100);
    LCD_Command(0x30);

    LCD_Command(0x20);  
    LCD_Command(0x28);  
    LCD_Command(0x08); 
    LCD_Command(0x01);  
    _delay_ms(2);
    LCD_Command(0x06);  
    LCD_Command(0x0C);  
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    LCD_Command(addr);
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    _delay_ms(2);
}
