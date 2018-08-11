#ifndef __DATA_H
#define __DATA_H	 		  
#include "sys.h" 
#include "stm32f10x.h"

extern u8 Data_Motor[5];
//Data_Motor[0]--MOTOR-1-SPEED
//Data_Motor[1]--MOTOR-2-SPEED
//Data_Motor[2]--MOTOR-3-SPEED
//Data_Motor[3]--MOTOR-4-SPEED
//Data_Motor[4]--[7:6-MOTOR4]--[5:4-MOTOR3]--[3:2-MOTOR2]--[2:4-MOTOR1]
/*
     00--Standby
		 01-CW
		 10-CCW
		 11--Brake
*/

#endif