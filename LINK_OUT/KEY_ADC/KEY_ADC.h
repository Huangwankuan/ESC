#ifndef _KEY_ADC_H
#define _KEY_ADC_H
#include "stm32f10x.h"
#include "ADC.h"
//IO对应关系
//ADC12_IN0---PA0---L-X-Axis
//ADC12_IN1---PA1---L-Y-Axis
//ADC12_IN2---PA2---R-X-Axis
//ADC12_IN3---PA3---R-Y-Axis


#define Forward 0x00
#define Backward 0x01
#define Left 0x02
#define Right 0x03
#define Go 0x04
#define Stop 0x00


void KEY_ADC_Initial(void);
u16 KEY_Calibration_Y(void);
u16 KEY_Calibration_X(void);
#endif
