#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define ON 0
#define OFF 1

#define LED_G  PAout(4) //
#define LED_R  PAout(3) //
#define LED_B  PAout(5)  //

void LED_Initial(void);
	
#endif
