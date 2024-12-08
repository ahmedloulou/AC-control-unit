#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <util/delay.h>

// Define clock speed
#define F_CPU 16000000UL
#define SCL_CLOCK 100000L

void I2C_Init(void);               // Initialize I2C
void I2C_Start(void);              // Send start condition
void I2C_Stop(void);               // Send stop condition
void I2C_Write(uint8_t data);      // Write byte to I2C
uint8_t I2C_ReadAck(void);         // Read byte with ACK
uint8_t I2C_ReadNack(void);        // Read byte with NACK

#endif
