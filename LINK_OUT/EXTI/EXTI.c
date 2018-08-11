#include "EXTI.h"

void EXTI_MOTOR_Phas_Initial(void)
{
	
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��AFIO����

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOA ʱ��
	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ�� A1��Ϊ�ж�������--�����ź�A

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);//ѡ�� A2��Ϊ�ж�������--�����ź�B

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//ѡ�� A3��Ϊ�ж�������--�����ź�C

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);//
	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//		
	
	
NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;			//ʹ���ⲿ�ж�ͨ�� 1
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 	

NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ���ⲿ�ж�ͨ�� 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ���ⲿ�ж�ͨ�� 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 	

NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ���ⲿ�ж�ͨ�� 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ���ⲿ�ж�ͨ�� 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 

NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ���ⲿ�ж�ͨ�� 3
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 


EXTI_DeInit();//Ĭ�Ͻ�EXTI����Ϊȱʡֵ

EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line1;//�ж���ѡ��
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�����ж�����
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش���
EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��
EXTI_Init(&EXTI_InitStructure);//����д��

EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line11;//�ж���ѡ��
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�����ж�����
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��
EXTI_Init(&EXTI_InitStructure);//����д��


EXTI->IMR |= EXTI_Line4;
EXTI->EMR |= EXTI_Line4;

EXTI->IMR |= EXTI_Line15;
EXTI->EMR |= EXTI_Line15;//��Ҫʹ�ÿ⺯�����ж����ã�������Ϊ�����ص��жϣ�����������IO�������

}
