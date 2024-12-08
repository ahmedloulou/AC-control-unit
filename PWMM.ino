#include <avr/io.h>
#include "PWM.h"

void PWM_init(){
  
  DDRD |= (1<<PD6);    
  
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);	
	TCCR0B |= (1<<CS00);	

  Set_DutyCycle(0);
}

void Set_DutyCycle(int duty){
    OCR0A = duty;
}
