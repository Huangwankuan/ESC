#include "UART.h"
#include "stdio.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void UART1_Initial(u32 boad_rate)
{
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	
GPIO_PinRemapConfig(AFIO_MAPR_USART1_REMAP, ENABLE);

	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

USART_InitStructure.USART_BaudRate = boad_rate;//波特率
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
USART_Init(USART1,&USART_InitStructure);

USART_Cmd(USART1, ENABLE);//使能USART外设

USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能指定的USART中断

NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

}

