#ifndef __MOTOR_H
#define __MOTOR_H


#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"


typedef struct{
	u8 direction;
	u16 SPEED;
	u8 Status;
}MOTOR_Status_TypeDef;

#define MOTOR_ON 0
#define MOTOR_OFF 1


#define MOTOR_STOP 0
#define MOTOR_CW 1
#define MOTOR_CCW 2
#define MOTOR_STANDBY 3

#define IN_A  PBout(15) //
#define EN_A  PAout(8) //

#define IN_B  PBout(13) //
#define EN_B  PBout(14) //

#define IN_C  PAout(10) //
#define EN_C  PBout(12) //

#define G1 PAin(0)
#define G2 PAin(1)
#define G3 PAin(2)

void MOTOR_GPIO_Initial(void);
void MOTOR_Phase(u8 num);
void MOTOR_HALL_Initial(void);
int MOTOR_Get_G(void);
void MOTOR_UP(int Status);
#endif

