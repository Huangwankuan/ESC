//****************************************************** 
//函数：
//目的：ADC配置
//说明：
//Maker：huangwankuan
//2017.2.7
//Version：V1.1
//All rights reserved
//******************************************************
#ifndef _ADC_h
#define _ADC_h
#include "stm32f10x.h"
#include "delay.h"

#define ADC_Con_Time 3


void ADC_Initial(void);
u16 ADC_Get_Average_Value(void);
void ADC_Start_Con(void);
u16 ADC_Get_Single_Value(void);
#endif


