#include "TIM.h"
void TIM_NVIC_PWM_Initial(void)
{

}

void TIM_PWM_Initial(u16 arr,u16 TIM_PWM_Pre,u16 value_PWM_Initial)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx	�Ĵ���
	
TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
TIM_TimeBaseStructure.TIM_Prescaler = TIM_PWM_Pre;//Ԥ��Ƶֵ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ��



TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ��
TIM_OCInitStructure.TIM_Pulse = value_PWM_Initial;//������,���������ռ�ձ�
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�ߵ�ƽ��Ч

TIM_OC2Init(TIM1,&TIM_OCInitStructure);//��ʼ��  OC1����ͨ��1

TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ�ʧ��TIM����Ԥװ�ؼĴ�����CCR2   Ԥװ�غ�����װ��

TIM_ARRPreloadConfig(TIM1,ENABLE);  //

TIM_CtrlPWMOutputs(TIM1,ENABLE);  //

TIM_Cmd(TIM1, ENABLE);//ʹ�ܻ���ʧ��TIMx����


}

void TIM2_Initial(u16 arr, u16 TIM_PWM_Pre)//
{
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
	
TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
TIM_TimeBaseStructure.TIM_Prescaler = TIM_PWM_Pre;//Ԥ��Ƶֵ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ��

TIM_ARRPreloadConfig(TIM2,ENABLE);  //ʹ���Զ���װ��
	

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ����  ��֤ ��������ת���������
NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//��Ӧ���ȼ���Ӧ���
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
	
//TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�

TIM_Cmd(TIM2, ENABLE);//ʹ�ܻ���ʧ��TIMx����

}



