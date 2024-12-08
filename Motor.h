#ifndef __DC_MOTOR
#define __DC_MOTOR
typedef enum {
    turnclockwise= 0,
    turnanticlockwise
} tenuDirection;

void DC_Init();
void DC_Start(unsigned char MotorID, tenuDirection Direction, unsigned char Speed);
void DC_Stop(unsigned char MotorID);

#endif
