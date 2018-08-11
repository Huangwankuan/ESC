#ifndef __TIM_H_
#define __TIM_H_
#include "stm32f10x.h"
void TIM_NVIC_PWM_Initial(void);
void TIM_PWM_Initial(u16 arr,u16 TIM_PWM_Pre,u16 value_PWM_Initial);
void TIM2_Initial(u16 arr, u16 TIM_PWM_Pre);//
void TIM2_IRQHandler(void);
#endif
