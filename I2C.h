#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <util/delay.h>

// Define clock speed
#define F_CPU 16000000UL
#define SCL_CLOCK 100000L

void I2C_Init(void);              
void I2C_Start(void);           
void I2C_Stop(void);              
void I2C_Write(uint8_t data);     
uint8_t I2C_ReadAck(void);      
uint8_t I2C_ReadNack(void);     
#endif
