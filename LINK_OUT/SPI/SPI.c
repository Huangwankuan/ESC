#include "SPI.h"

void SPI1_Initial(void)
{
	
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟

RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//使能或者失能APB1外设时钟
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx寄存器

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx寄存器

SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//单线双向发送
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置为主SPI
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位帧结构
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//时钟悬空高
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//第二个时钟沿
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//内部NSS信号由SSI位控制816;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//预分频值为8  即为9MHZ
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
SPI_InitStructure.SPI_CRCPolynomial = 7;//用于CRC值计算的多项式
SPI_Init(SPI1,&SPI_InitStructure);//初始化配置

SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
SPI1_Read_Write_Byte(0xff);//启动传输		 
 
GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);  //PB13/14/15上拉

}   

//SPIx 读写一个字节
//TxData:要写入的字节
//返回0  产生ERROR
u8 SPI1_Read_Write_Byte(u8 Data)
{		
	u8 feedback=0;			

	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		feedback++;
		if(feedback>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, Data); //通过外设SPIx发送一个数据   SPI只发送数据不接收数据
		
		
	feedback=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		feedback++;
		if(feedback>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据									    				    

	
	}
