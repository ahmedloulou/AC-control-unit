#include <util/delay.h>
#include "analogdigital.h"
#include "Lcd.h"
#include "Uart.h"
#include "DC_Motor.h"
#include <stdlib.h>
#if !defined(__AVR_ATmega328P__)
#include <avr/iom328p.h>
#endif

int main(void) {
  unsigned short adcreading;
  unsigned short upperlimit = 700;
  unsigned short lowerlimit = 400;
  unsigned char buffer[4];
  unsigned char control_limit = 0;

  DDRD &= ~(1 << PD0);
  PORTD |= (1 << PD0);
  DDRB &= ~(1 << PB4);
  PORTB |= (1 << PB4);
  DDRD &= ~(1 << PD2);
  PORTD |= (1 << PD2);

  Adc_Init();
  Uart_Init();
  LCD_Init();

  while (1) {
    _delay_ms(200);

    adcreading = Adc_ReadChannel(1);
    itoa(adcreading, buffer, 10);
    Uart_SendString(buffer, 4);
    Uart_SendChar('\n');

    LCD_Clear();
    LCD_String("Sensor: ");
    LCD_String(buffer);
    LCD_Command(0xC0);
    itoa(upperlimit, buffer, 10);
    LCD_String("UL: ");
    LCD_String(buffer);
    LCD_String("  LL: ");
    itoa(lowerlimit, buffer, 10);
    LCD_String(buffer);

    if (((PIND >> 0) & 1) == 0) {
      if (control_limit == 0) {
        upperlimit -= 10;
      } else {
        lowerlimit -= 10;
      }
      _delay_ms(1000);
    }

    if (((PINB >> 4) & 1) == 0) {
      if (control_limit == 0) {
        upperlimit += 10;
      } else {
        lowerlimit += 10;
      }
      _delay_ms(1000);
    }

    if (((PIND >> 2) & 1) == 0) {
      control_limit = !control_limit;
      _delay_ms(1000);
    }

    if (adcreading >= lowerlimit && adcreading < upperlimit) {
      LCD_Command(0x8E);
      LCD_String("OK");
      _delay_ms(1000);
      DC_Start(0, DIRECTION_CW, 0);
      _delay_ms(500);
      DC_Stop(0);
      _delay_ms(1000);
    } else {
      LCD_Command(0x8D);
      LCD_String("NOK");
      _delay_ms(1000);
      DC_Start(0, DIRECTION_CCW, 0);
      _delay_ms(500);
      DC_Stop(0);
      _delay_ms(1000);
    }
  }

  return 0;
}
