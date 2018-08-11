#include "LED.h"

void LED_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
	
	LED_R=OFF;	
	LED_G=OFF;	
	LED_B=OFF;	
	
}
