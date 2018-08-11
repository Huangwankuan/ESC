#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define KEY_1  PAin(6) //
#define KEY_2  PAin(11) //

#define KEY_DOWN 1
#define KEY_UP 0

void KEY_Initial(void);
u8 KEY_FB(void);

#endif
