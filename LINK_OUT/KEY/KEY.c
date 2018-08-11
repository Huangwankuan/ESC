#include "KEY.h"

void KEY_Initial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器

	
}
/*
* 0 KEY1=未按下 KEY2=未按下 
* 1 KEY1=未按下 KEY2=按下 
* 2 KEY1=按下 KEY2=未按下 
* 3 KEY1=按下 KEY2=按下 
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
