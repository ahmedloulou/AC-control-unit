#include <util/delay.h> // we include the library for the delay.
#include "analogdigital.h" // we include the header file for the analog digital conversion.
#include "Motor.h" // we include the header file for the dc motor.
#include "LCD.h"  //  we include the header file for the LCD screen.
#include "PWM.h" //  we include the header file for the PWM.
#include <EEPROM.h> //  we include the built in file for the EEPROM
#include <stdlib.h> // we include the built in file for the conversion to ascii code.

int upperaddress = 10; // define the address that stores the value of the upperlimit in the eeprom.
int loweraddress = 0 ; // define the address that stores the value of the lowerlimit in the eeprom.

int main(void) {     // start of the main function of the code int as it returns an integer at the end and we use void as it doesnt require any arguements
    unsigned short tempsensor; // define a variable named tempsensor as unsigned"only holds positive integers" and short "as it only holds numbers up to 16bit to require less memory"
    unsigned short upperlimit = EEPROM.read(upperaddress); // define a variable named upperlimit that holds the value stored in the upperaddress in the eeprom using the function eeprom.read
    unsigned short lowerlimit = EEPROM.read(loweraddress); // define a variable named lowerlimit that holds the value stored in the loweraddress in the eeprom using the function eeprom.read
    unsigned short MotorSpeed = 255; // define the value of the motorspeed as 255 "maximum speed"
    unsigned char buffer[5]; // define the buffer array to hold 5 elements
    float TemperatureValue; // define a variable temperaturevalue as a float"holds a number with a decimal place"
    float refrenceVoltage; // define a variable refrencevoltage as a float

    DDRB &= ~(1 << PB2); // initialize pin PB2 which is digital pin 10 as an input.
    PORTB |= (1 << PB2); // activate the internal pull up resistor in digital pin 10.
    DDRB &= ~(1 << PB3); // initialize pin PB3 which is digital pin 11 as an input.
    PORTB |= (1 << PB3); // activate the internal pull up resistor in digital pin 11.
    DDRB &= ~(1 << PB4); // initialize pin PB4 which is digital pin 12 as an input.
    PORTB |= (1 << PB4); // activate the internal pull up resistor in digital pin 12.
    DDRB &= ~(1 << PB5); // initialize pin PB5 which is digital pin 13 as an input.
    PORTB |= (1 << PB5); // activate the internal pull up resistor in digital pin 13.

    Adc_Init();  // include the initialization of the adc from the header file analogdigital.h
    Uart_Init(); // include the initialization of the uart communiaction protocol from the header file Uart.h 
    LCD_Init();  // include the initialization of the LCD screen which uses I2C communication protocol from the header file LCD.h
    PWM_init();  // include the initialization of the PWM from the header file PWM.h
    DC_Init();   // include the initialization of the motor connected from the header file Motor.h 

    while (1) {   // start an infinte loop to keep the code always running.
        _delay_ms(200); // add a delay to make the code run smoother.
        tempsensor = Adc_ReadChannel(1);  // we assigned the value comming from the temperature sensor, using the function"Adc_ReadChannel", to the variable tempsensor
        refrenceVoltage = 500.0 ; // set the refrence voltage to 500 o convert the voltage to temperature
        TemperatureValue = tempsensor * (refrenceVoltage / 1024.0); // use the lm35 equation to calculate the temperature
        itoa(TemperatureValue, buffer, 10); // function itoa takes 3 arguemts and converts the input to a string. first arguement is the input, second is the place of store, third is the base system
        Uart_SendString(buffer, 4); // we use uart sendstring to display the value on the serial monitor for double checking the sensor input it takes 2 arguemnts the input and its length
        Uart_SendChar('\n'); // // we use uart sendchar to sen a character in this \n which shifts the cursor to the next row for easier readability

        LCD_Clear(); // we use this function to clear the lcd from anything that was on it from before
        LCD_SetCursor(0, 0); // we use function lcd setcursor to shift the cursor to the first row and first coloumn on the lcd
        LCD_String("Temp: "); // function lcd string is used to display the words sensor
        LCD_String(buffer); // we then use lcd string again to display thr valur of thr temperature that is stored in the buffer from thr function itoa
        LCD_String(" C "); // display the letter C which means degrees celsius
        LCD_SetCursor(1, 0); // we then move the cursor to the bottom row to start typing more things
        itoa(upperlimit, buffer, 10); // itoa is used once again to convert the upper limit to a string as the lcd cant display it in its original form
        LCD_String("UL: "); // before displaying the upperlimit value we display UL to be understand easily when looking at the lcd that this is the upper limit
        LCD_String(buffer); // display the value of the upperlimit
        LCD_String(" LL: "); // we then go on to the lower limit we send LL for the same reason as the upperlimit 
        itoa(lowerlimit, buffer, 10); // convert the value of the lowerlimit "each time we use itoa we overwrite in the buffer meaning nothing gets written in the wrong place"
        LCD_String(buffer); // display the value of the lowerlimit  

        if (((PINB >> PB2) & 1) == 0) { // we use this if condition to check if the push button is pressed to adjust the limit
            lowerlimit -= 5; // if buttton is pressed lower limit decreases by 5
            EEPROM.update(loweraddress, lowerlimit);  // we then use the function eeprom update to store the new value of the lower limit in the loweraddress
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


        if (TemperatureValue >= lowerlimit && TemperatureValue < upperlimit) {  //if ondition is used to check if the temeprature value is between the limits or not
            LCD_SetCursor(0, 13); // we move the cursor to coloumn 13 in the top row
            LCD_String("OK"); // we then display there the letters OK to identify that it is within range
            DC_Start(0, turnclockwise, MotorSpeed);  // then we rotate the motor using the function dc start which takes 3 arguements first is motor id which is which motor to power, then the direction clockwise or anti finally the speed
            _delay_ms(1000); // this delay identifies how long the motor spins for which in this case 1 second
            DC_Stop(0); // we then stop the motor using function dc stop which takes motor id as the only arguement
            _delay_ms(1000); // then this delay is responsible for how long the motor stays stop
        } else {
            LCD_SetCursor(0, 13);
            LCD_String("NOK");
            DC_Start(0, turnanticlockwise, MotorSpeed);
            _delay_ms(1000);
            DC_Stop(0);
            _delay_ms(1000);
        }
    }

    return 0;
}
