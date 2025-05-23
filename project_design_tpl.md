# Detailed Design Document

| **Author**              | `Ahmed Loulou, Sherif Helmy, Youssef Abouelkheir`                                       |
|:------------------------|:-----------------------------------------------------|
| **Status**              | `Inspection`                          |
| **Version**             | `1.0`                                                |
| **Date**                | `07/12/2024`                                         |

## Introduction

The aim from this report is to show the importance of embedded systems and explain how it is going to be used to an automotive air condition system. This project concentrates on detecting temperature values, controlling a fan motor and storing user settings. The following system displays the information on a 16*2 LCD screen, and it gives the user the ability to change its parameters by using a keypad. The purpose of this project is to assemble an efficient climate control system where the temperature can be displayed and saved, the direction of the fan can be changed in addition to its speed, and the parameters of the circuit will be present on the LCD. To realize all of this the system requires the usage of a keypad to set the limits of the temperature, a motor to have control over the fan and EEPROM to save the settings. The following parameters like the temperature and the state of the fan will be shown on the LCD. We used the C programming language to achieve this project in addition to it we used Uart and I2C communication, PWM module to control speed and analog digital conversion system. This circuit is based on real air conditioning systems found vehicles, highlighting the usage of embedded systems for practical applications. For example, the fan motor control of this project is near the airflow direction and speed of modern vehicles. The temperature settings and display are like the ones found in our everyday climate control systems, making this project a near eq~uivalent demonstration of how this technology works.


### Purpose
This low-level design specification describes the internal structure and the functions of the software module. It describes the interaction, data structures, specific embedded constraints and background information about design. This specification is the basis for coding and is an input document for the corresponding module test specification. The terms Low-level design specification and module specification are used interchangeably in Valeo to refer to documents of this type.

### Scope
This report describes how an Arduino-based system works and how it was designed. The system uses a temperature sensor as an analogue input, an LCD as an output display, a fan motor which its speed is controlled by a pwm and two push buttons to change the upper and lower limit ranges on the fly and third push button to toggle between limits.The temperature sensor gives an analogue signal to the Arduino. The built-in ADC turns the signal into a digital value, which is then shown on a 16x2 LCD. The system permits the user to set these varying limits and see them along with the sensor's current value. This system models real-world air conditioning system that are used in modern day transportations. This file is a basic outline for the software's framework, how its components work together, and any built-in restrictions. It will help with future development and testing.


## Architectural Overview

This section describes where this module resides in the context of the software architecture
```plantuml
@startuml
rectangle AC_control_System {
    rectangle "Analog digital convertor Module"
    rectangle "LCD Module"
    rectangle "UART communication Module"
    rectangle "main application Logic"
    rectangle "I2C Module"
    rectangle "Motor Module"
    rectangle "PWM Module"

    "main application Logic" --> "I2C Module" : communicates
    "main application Logic" --> "UART communication Module" : communicates
    "I2C Module" --> "LCD Module" : communicates
    "UART communication Module" --> "Analog digital convertor Module" : communicates
    "Analog digital convertor Module" --> "PWM Module" : communicates
    "PWM Module" --> "Motor Module" : communicates
}
@enduml

```

### Assumptions & Constraints
Indicate constraints of the low level design and any assumptions appropriate for the design.
```plantuml
@startuml
(*) --> initialization
--> configuration
if upperlimit >value > lowerlimit 
  --> display on lcd "OK", motor rotates clockwise
  --> (*)
else
  --> display on lcd "NOK", motor rotates anticlockwise
endif
@enduml
```

## Functional Description
This code takes the value of an analogue sensor, compares it to upper and lower limits that can be set, and shows the result on an LCD screen. It sets up the ADC, UART, and LCD modules and their starting values for upperlimit and lowerlimit after setting up certain pins as inputs for the push buttons. The code reads the sensor value from the ADC wich is an integer and turns it into a string, sends it over UART, and shows the string along with the current limits on the LCD in the main loop. Users can change these limits on the fly by pressing buttons. The code makes a motor rotate clock wise direction and displays "OK" on the LCD if the sensor reading is within the given range,the speed of the motor will be determined by the pwm.If it's not,the motor will rotate anticlock wise direction and will display "NOK" on the LCD,the speed of the motor will be determined by the pwm.This demonstrates how to watch and control an analogue input in real time with values that can be changed.


## Implementation of the Module
This project uses Arduino to demonstrate a motor movement powered by a battery, analog-to-digital conversion (ADC) and display the digital signal on an LCD with changeable limit ranges via buttons. A temperature sensor that gives the analogue signal, an LCD screen to show data, buttons for interactive range limit control, and a motor driver and a battery for the motor, these are the main hardware components. The Arduino's ADC digitises the sensors's analogue variable input signal. This digital reading and user-defined upper and lower limits are shown on a 16x2 LCD screen.

The system continuously reads the sensor's output and checks for limitations using the ADC. Three buttons let users dynamically alter these limits: two for the higher limit and two for the lower limit and the final button to toggle between both limits. A normal state is indicated by an "OK" message on the LCD and the motor will rotate clockwise direction if the input value is wtihin the limit range.The motor will rotate anticlock wise and  a "NOK" message will appear on the LCD to inform the user if the input value is outside this range.

This configuration simulates real-world air conditining systems. In a heating or cooling system, the temperature sensor acts as a thermostat dial and the ADC would shouw the temperature setting into a digital display. In this example, the LCD would display the specified temperature and the upper and lower limits in real time. The temperature sensor can act as a switch for the control of direction of the motor wich will rotate clock wise or anticlock wise depending on the value.The variable change of the limits in both examples shows how users can interactively control and monitor analogue values in digital formats.

This paper describes each software module's structure and functions as a low-level design specification. The system's underlying workings are explained by covering component interactions, embedded data structures, and design limitations. This design document establishes each component's functionality and role in the system, guiding coding and testing.


## Integration and Configuration
### Static Files
Typically a module consists of C and H files but other file types may exist. List all files that form this module

| File name | Contents                             |
|-----------|--------------------------------------|
| ACcontrolUnit.ino   | Includes the main code for the apllication |
| analogdigital.h | header for adc functions        |
| Aanalogdigital.ino | Includes all adc functions and initlization         |
| LCD.h     | header for lcd functions                |
| LCD.ino    | Includes all Lcd functions and initlization                |
| Uart.h    | header for uart communications functions |
| Uart.ino   | Includes all uart communications functions and initlization |
| I2C.h | header the inisialization of the I2C communication       |
| I2C.ino | include all functions required in the I2C        |
| PWM.h | header for the PWM functions        |
| PWM.ino | includes all pwm functions required     |
| Motor.h | header for the motor functions        |
| Motor.ino | includes all motor functions required to make the motor spin       |
### Include Structure

If there is a complex file structure e.g. more than one c-file or more than two h-files use a diagram to explain the relationship between the source and dependent include files.

```plantuml
@startuml
package "pkg" {
    [Aanalogdigital.ino]...>[analogdigital.h] : includes
    [Lcd.ino]...>[Lcd.h] : includes
    [Uart.ino]...>[Uart.h] : includes
    [I2C.ino]...>[I2C.h] : includes
    [PWM.ino]...>[PWM.h] : includes
    [motor.ino]...>[motor.h] : includes
    [ACcontrolUnit.ino] --> [analogdigital.h] : includes
    [ACcontrolUnit.ino] --> [Lcd.h] : includes
    [ACcontrolUnit.ino] --> [Uart.h] : includes
    [ACcontrolUnit.ino] --> [I2C.h] : includes
    [ACcontrolUnit.ino] --> [PWM.h] : includes
    [ACcontrolUnit.ino] --> [motor.h] : includes
    interface Interf3
}
@enduml
```

### Configuration
Any required hardware/software or system configuration that can be adjusted a header file shall be detailed in this section. This can be placed in the table below.
| Name | Value range | Description |
|------|-------------|-------------|
|   limit   |     lowerlimit - upperlimit        |     Uses upper and lower limit to create a range        |
|   lowerlimit   |     loweraddress        |   uses the eeprom memory to store the value of the limit        |
|   upperlimit   |     upperaddress        |   uses the eeprom memory to store the value of the limit        |
|   Baudrate  |     9600       |     sets the baudrate       |
|   F_CPU  |     16 MHz       |    sets the CPU clock frequency for I2C communication      |
|   SCL_CLOCK  |     100 kHz       |     sets the clock speed for the I2C communication "frequency of the SCL line"       |
|   refrenceVoltage  |     500      |     used in calculations of temperature from the sensor      |
|   MotorSpeed  |     255       |     sets the speed of the motor       |
