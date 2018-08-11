#include "EXTI.h"

void EXTI_MOTOR_Phas_Initial(void)
{
	
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开AFIO功能

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA 时钟
	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择 A1作为中断线输入--霍尔信号A

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);//选择 A2作为中断线输入--霍尔信号B

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//选择 A3作为中断线输入--霍尔信号C

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);//
	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//		
	
	
NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;			//使能外部中断通道 1
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 	

NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能外部中断通道 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能外部中断通道 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 	

NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能外部中断通道 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能外部中断通道 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能外部中断通道 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 


EXTI_DeInit();//默认将EXTI均设为缺省值

EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line1;//中断线选择
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//产生中断请求
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能
EXTI_Init(&EXTI_InitStructure);//数据写入

EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line11;//中断线选择
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//产生中断请求
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//跳变沿触发
EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能
EXTI_Init(&EXTI_InitStructure);//数据写入


EXTI->IMR |= EXTI_Line4;
EXTI->EMR |= EXTI_Line4;

EXTI->IMR |= EXTI_Line15;
EXTI->EMR |= EXTI_Line15;//不要使用库函数的中断配置，会配置为跳变沿等中断，会引入其他IO的误操作

}
