#include <util/delay.h> 
#include "analogdigital.h" 
#include "Motor.h" 
#include "LCD.h"  
#include "PWM.h"
#include <EEPROM.h> 
#include <stdlib.h> 

int upperaddress = 10; 
int loweraddress = 0 ; 

int main(void) {    
    unsigned short tempsensor;
    unsigned short upperlimit = EEPROM.read(upperaddress);
    unsigned short lowerlimit = EEPROM.read(loweraddress);
    unsigned char buffer[5];
    float TemperatureValue;
    float refrenceVoltage;
    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);
    DDRB &= ~(1 << PB3);
    PORTB |= (1 << PB3);
    DDRB &= ~(1 << PB4);
    PORTB |= (1 << PB4);
    DDRB &= ~(1 << PB5);
    PORTB |= (1 << PB5);

    Adc_Init();
    Uart_Init();
    LCD_Init();
    PWM_init();       
    DC_Init();      

    while (1) {
        _delay_ms(200);
        tempsensor = Adc_ReadChannel(1);
        refrenceVoltage = 500.0 ;
        TemperatureValue = tempsensor * (refrenceVoltage / 1024.0);
        itoa(TemperatureValue, buffer, 10);
        Uart_SendString(buffer, 4);
        Uart_SendChar('\n');

        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_String("Sensor: ");
        LCD_String(buffer);
        LCD_SetCursor(1, 0);
        itoa(upperlimit, buffer, 10);
        LCD_String("UL: ");
        LCD_String(buffer);
        LCD_String(" LL: ");
        itoa(lowerlimit, buffer, 10);
        LCD_String(buffer);

        if (((PINB >> PB2) & 1) == 0) {
            lowerlimit -= 5;
            EEPROM.update(loweraddress, lowerlimit);
            _delay_ms(300);
        }

        if (((PINB >> PB3) & 1) == 0) {
            lowerlimit += 5;
            EEPROM.update(loweraddress, lowerlimit);
            _delay_ms(300);
        }

        if (((PINB >> PB4) & 1) == 0) {
            upperlimit -= 5;
            EEPROM.update(upperaddress, upperlimit);
            _delay_ms(300);
        }

        if (((PINB >> PB5) & 1) == 0) {
            upperlimit += 5;
            EEPROM.update(upperaddress, upperlimit);
            _delay_ms(300);
        }

        if (TemperatureValue >= lowerlimit && TemperatureValue < upperlimit) {
            LCD_SetCursor(0, 13);
            LCD_String("OK");
            DC_Start(0, turnclockwise, 255);
            _delay_ms(500);
            DC_Stop(0);
            _delay_ms(500);
        } else {
            LCD_SetCursor(0, 13);
            LCD_String("NOK");
            DC_Start(0, turnanticlockwise, 255);
            _delay_ms(500);
            DC_Stop(0);
            _delay_ms(500);
        }
    }

    return 0;
}

