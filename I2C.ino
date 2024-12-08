#include "I2C.h"

void I2C_Init(void) {
    TWSR = 0x00;                              // Set prescaler to 1
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;    // Set bit rate
    TWCR = (1 << TWEN);                       // Enable I2C
}

void I2C_Start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));           // Wait for start condition
}

void I2C_Stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    _delay_us(10);                            // Short delay after stop
}

void I2C_Write(uint8_t data) {
    TWDR = data;                              // Load data into data register
    TWCR = (1 << TWEN) | (1 << TWINT);        // Start transmission
    while (!(TWCR & (1 << TWINT)));           // Wait for completion
}

uint8_t I2C_ReadAck(void) {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_ReadNack(void) {
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

