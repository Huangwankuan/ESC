#include "LED.h"

void LED_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx	�Ĵ���
	
	LED_R=OFF;	
	LED_G=OFF;	
	LED_B=OFF;	
	
}
