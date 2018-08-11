#include "KEY.h"

void KEY_Initial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx	�Ĵ���

	
}
/*
* 0 KEY1=δ���� KEY2=δ���� 
* 1 KEY1=δ���� KEY2=���� 
* 2 KEY1=���� KEY2=δ���� 
* 3 KEY1=���� KEY2=���� 
*/


u8 KEY_FB(void)//0 KEY1
{
	if((KEY_1==0)&&(KEY_2==0))
	{
		delay_ms(5);
		if((KEY_1==0)&&(KEY_2==0))
		{
			return 0;
		}
	}
	
	else if((KEY_1==0)&&(KEY_2==1))
	{
		delay_ms(5);
		if((KEY_1==0)&&(KEY_2==1))
		{
			return 1;
		}
	}
	
	else if((KEY_1==1)&&(KEY_2==0))
	{
			delay_ms(5);
		if((KEY_1==1)&&(KEY_2==0))
		{
			return 2;
		}
	}
	
	else if((KEY_1==1)&&(KEY_2==1))
	{
		delay_ms(5);
		if((KEY_1==1)&&(KEY_2==1))
		{
			return 3;
		}
	}
	else 
	{
		return 0;
	}
	return 0;
}
