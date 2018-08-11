#include "TIM.h"
void TIM_NVIC_PWM_Initial(void)
{

}

void TIM_PWM_Initial(u16 arr,u16 TIM_PWM_Pre,u16 value_PWM_Initial)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能或者失能APB1外设时钟
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
	
TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
TIM_TimeBaseStructure.TIM_Prescaler = TIM_PWM_Pre;//预分频值
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化



TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能
TIM_OCInitStructure.TIM_Pulse = value_PWM_Initial;//脉冲宽度,由这个设置占空比
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高电平有效

TIM_OC2Init(TIM1,&TIM_OCInitStructure);//初始化  OC1表明通道1

TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或失能TIM外设预装载寄存器在CCR2   预装载和马上装载

TIM_ARRPreloadConfig(TIM1,ENABLE);  //

TIM_CtrlPWMOutputs(TIM1,ENABLE);  //

TIM_Cmd(TIM1, ENABLE);//使能或者失能TIMx外设


}

void TIM2_Initial(u16 arr, u16 TIM_PWM_Pre)//
{
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能或者失能APB1外设时钟
	
TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
TIM_TimeBaseStructure.TIM_Prescaler = TIM_PWM_Pre;//预分频值
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化

TIM_ARRPreloadConfig(TIM2,ENABLE);  //使能自动重装载
	

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级最高  保证 不发生堵转大电流现象
NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//响应优先级对应最高
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
	
//TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断

TIM_Cmd(TIM2, ENABLE);//使能或者失能TIMx外设

}



