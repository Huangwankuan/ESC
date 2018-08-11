#include "OLED_SSD1322.h"
#include "FONT.h"
u8 OLED_GRAM_USER[64][32];
void SPI2_Init(void)
{
	
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能或者失能APB2外设时钟

RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//使能或者失能APB1外设时钟

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//单线双向发送
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置为主SPI
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位帧结构
SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//时钟悬空高
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//第二个时钟沿
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//内部NSS信号由SSI位控制
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//预分频值为2  预分频32以上会出现断点
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
SPI_InitStructure.SPI_CRCPolynomial = 7;//用于CRC值计算的多项式
SPI_Init(SPI2,&SPI_InitStructure);//初始化配置


SPI_Cmd(SPI2, ENABLE); //使能SPI外设
//////复位OLED
RES_OLED=1;
delay_ms(10);
RES_OLED=0;
delay_ms(50);
RES_OLED=1;
SPI2_WriteByte(0xff);//启动传输		 
 
GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_11);  //PB13/14/15上拉

}   

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_WriteByte(u8 SendData)
{		
	u8 feedback=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		feedback++;
			if(feedback>200)return 0;
		}
	SPI_I2S_SendData(SPI2, SendData); //通过外设SPIx发送一个数据   SPI只发送数据不接收数据
	return 1;				    				    
}

void OLED_Write_Data(u8 Write_Data_OLED,u8 decide)
{
	
	 NSS_SPI=0;
	if(decide==1)//1写命令  0写数据
	{                                                                                                                   

		SPI2_WriteByte(Write_Data_OLED);
				CMD_OLED=0;
	}
	else
	{

		SPI2_WriteByte(Write_Data_OLED);
				CMD_OLED=1;
	}
	NSS_SPI=1;
}
//开启OLED显示
void OLED_Display_On(void)
{
	//OLED_Write_Data(0X8D,OLED_CMD);  //电荷泵
	//OLED_Write_Data(0X14,OLED_CMD);  //ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
//	OLED_Write_Data(0X8D,OLED_CMD);  //电荷泵
	//OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}
void OLED_Reset_Initial(void)
{
	
	RES_OLED=0;
	delay_ms(50);
	RES_OLED=1;

	OLED_Display_Off();
	
	OLED_Write_Data(0XFD,OLED_CMD); 		//设置命令锁
	OLED_Write_Data(0X12,OLED_DATA);   //0X12开锁  0X16锁定
	
	OLED_Write_Data(0XB3,OLED_CMD); 		//设置时钟分频因子,震荡频率
	OLED_Write_Data(0X91,OLED_DATA);   //[3:0],分频因子;[7:4],震荡频率
	
	OLED_Write_Data(0XCA,OLED_CMD); 		//设置复用引脚比率
	OLED_Write_Data(63,OLED_DATA);   //2^6=64 
	
	OLED_Write_Data(0XA1,OLED_CMD);		 	//设置显示起止线  
	OLED_Write_Data(0x00,OLED_DATA);   //
	
	OLED_Write_Data(0XA2,OLED_CMD);		 	//设置显示偏移
	OLED_Write_Data(0X00,OLED_DATA);   //
	
	OLED_Write_Data(0XA0,OLED_CMD);		 	//设置映射和段COM模式
	OLED_Write_Data(0X10,OLED_DATA);   //  
	OLED_Write_Data(0X01,OLED_DATA);   //
	
	OLED_Write_Data(0XB5,OLED_CMD);		 	// SET GPIO 
	OLED_Write_Data(0X00,OLED_DATA);   //DISABLE  禁用
	
	OLED_Write_Data(0XAB,OLED_CMD);		 	//  VDD选择位
	OLED_Write_Data(0X01,OLED_DATA);   //   选择外部VDD
	
	OLED_Write_Data(0XB4,OLED_CMD);		 	//  显示改善A  
	OLED_Write_Data(0XA0,OLED_DATA);   //   使能VSL
	OLED_Write_Data(0XF8,OLED_DATA);   //    增强 低GS显示质量
	
	OLED_Write_Data(0XC1,OLED_CMD);		 	//  设置对比度  
	OLED_Write_Data(200,OLED_DATA);   //  0x00-0xff
	
	OLED_Write_Data(0XC7,OLED_CMD);		 	//   设置亮度
	OLED_Write_Data(200,OLED_DATA);   //  0xf0-0xff  
	
		OLED_Write_Data(0xB8,OLED_CMD); //  	 设置灰度表
		OLED_Write_Data(12,OLED_DATA); //     GS1  12
		OLED_Write_Data(24,OLED_DATA); //     GS2  24
		OLED_Write_Data(36,OLED_DATA); //     GS3  36
		OLED_Write_Data(48,OLED_DATA); //     GS4  48
		OLED_Write_Data(60,OLED_DATA); //     GS5  60
		OLED_Write_Data(72,OLED_DATA); //     GS6  
		OLED_Write_Data(84,OLED_DATA); //     GS7  
		OLED_Write_Data(96,OLED_DATA); //     GS8  
		OLED_Write_Data(108,OLED_DATA); //     GS9  
		OLED_Write_Data(120,OLED_DATA); //     GS10  
		OLED_Write_Data(132,OLED_DATA); //     GS11  
		OLED_Write_Data(144,OLED_DATA); //     GS12  
		OLED_Write_Data(156,OLED_DATA); //     GS13  
		OLED_Write_Data(168,OLED_DATA); //     GS14  
		OLED_Write_Data(180,OLED_DATA); //     GS15  
 		OLED_Write_Data(0x00,OLED_CMD); 	// 使能灰度表
	
	
	OLED_Write_Data(0XB1,OLED_CMD);		 	//  设置相的长度
	OLED_Write_Data(0XE2,OLED_DATA);   //

	
	OLED_Write_Data(0XD1,OLED_CMD);		 	//    显示改善B 
	OLED_Write_Data(0XA2,OLED_DATA);   //     正常 normal
	OLED_Write_Data(0X20,OLED_DATA);   //      
	
	OLED_Write_Data(0XBB,OLED_CMD);		 	//    设置预充电电压
	OLED_Write_Data(0X1F,OLED_DATA);   //     0.6*VCC
	
	OLED_Write_Data(0XB6,OLED_CMD);		 	//     设置第二个预充电周期
	OLED_Write_Data(0X08,OLED_DATA);   //    

	
	OLED_Write_Data(0XBE,OLED_CMD);		 	//    设置V-com电压  
	OLED_Write_Data(0X07,OLED_DATA);   //			 0.86*VCC
	
//	OLED_Write_Data(0XA4,OLED_CMD);		 	//   全部显示关闭  GS-Level 0  最低灰度级别
//	OLED_Write_Data(0XA5,OLED_CMD);		 	//   全部显示打开  GS-Level 15 最高灰度级别
	OLED_Write_Data(0XA6,OLED_CMD);		 		//   正常显示模式 
//	OLED_Write_Data(0XA7,OLED_CMD);		 	//   反相显示模式   详见Datasheet
	
//OLED_Write_Data(0XA8,OLED_CMD);		 	//   部分显示模式
	
	OLED_Write_Data(0XA9,OLED_CMD);		 	//    退出部分显示模式
	
	OLED_Display_On();
	
	//	OLED_Display_Off();
	OLED_Clean();
	
}

void OLED_Write_USER(void)
{
	u8 i,n;
			for(i=0;i<32;i++)
	{
		for(n=0;n<32;n++)
		{
			OLED_GRAM_USER[i][n]=0XF0;
		}
	}
}
void OLED_Refresh_Gram_General(u8 Gram_NUM)
{
	u8 i,n;
	OLED_Write_Data (0x15,OLED_CMD);  //设置列地址     0-119  
	OLED_Write_Data(0x1C,OLED_DATA);
	OLED_Write_Data(0X5B  ,OLED_DATA);   // 0-64    64*4 =256
	
	OLED_Write_Data (0x75,OLED_CMD);   //设置行地址    0-127
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0X3F,OLED_DATA);
	
	OLED_Write_Data(0x5C,OLED_CMD);			// 使能MCU写入GRAM
	
	Refresh_1_4_Gram(Gram_NUM);
	/*	for(i=0;i<32;i++)
	{
		for(n=0;n<128;n++)
		{
			OLED_Write_Data(OLED_GRAM_OLED[i][n],OLED_DATA);
		//	OLED_Write_Data(0xff,OLED_DATA);
		//	OLED_Write_Data(0x00,OLED_DATA);
		}
	}*/
	
	
	}
void BMP_4_TEST(void)
{
	  	u8 i,n;
			for(i=0;i<64;i++)
	{
		for(n=0;n<32;n++)
		{
			OLED_GRAM_USER[i][n]=BMP_4[i][n];
		}
	}
}
void Refresh_1_4_Gram(u8 Gray_num)  //0-15  0灭  15灰度最高
{
	u8 a=Gray_num%16;
	u8 i,j,n;
	u8 Buf_data,Buf_data_1,Buf_data_2;
								for(i=0;i<64;i++)
								{
									   for(j=0;j<32;j++)
									{
										  for(n=0;n<4;n++)
										{
											Buf_data=OLED_GRAM_USER[i][j];
											//Buf_data=0x84;
											switch(n)
											{
												case 1:
																if((Buf_data&0x80)==0x80)Buf_data_1=a;
																else	Buf_data_1=0x00;		
																if((Buf_data&0x40)==0x40)Buf_data_2=a<<4;
																else	Buf_data_2=0x00;
																break;
												case 0:
																if((Buf_data&0x20)==0x20)Buf_data_1=a;
																else	Buf_data_1=0x00;		
																if((Buf_data&0x10)==0x10)Buf_data_2=a<<4;
																else	Buf_data_2=0x00;
																break;
												case 3:
																if((Buf_data&0x08)==0x08)Buf_data_1=a;
																else	Buf_data_1=0x00;		
																if((Buf_data&0x04)==0x04)Buf_data_2=a<<4;
																else	Buf_data_2=0x00;
																break;
												case 2:
																if((Buf_data&0x02)==0x02)Buf_data_1=a;
																else	Buf_data_1=0x00;		
																if((Buf_data&0x01)==0x01)Buf_data_2=a<<4;
																else	Buf_data_2=0x00;
																break;
											}						
											//	OLED_Write_Data(0xf0,OLED_DATA);	
											
										OLED_Write_Data(Buf_data_1|Buf_data_2,OLED_DATA);
										//	OLED_GRAM_OLED[i][j*4+n]=Buf_data_1|Buf_data_2;
										}
								/*OLED_Write_Data(0xf0,OLED_DATA);
								OLED_Write_Data(0x00,OLED_DATA);
								OLED_Write_Data(0x0f,OLED_DATA);
								OLED_Write_Data(0x00,OLED_DATA);*/
									}
								}
}


void OLED_Test(void)//
{
			u8 i,n;
	 for(i=0;i<64;i++)
	{
		for(n=0;n<32;n++)
		{
			OLED_GRAM_USER[i][n]=0xF0;
		}
	}
}

void OLED_Clean(void)
{
	u8 i,n;
	OLED_Write_Data (0x15,OLED_CMD);  //设置列地址     0-119  
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0x77,OLED_DATA);
//	OLED_Write_Data(0x00,OLED_DATA);
	
	
	OLED_Write_Data (0x75,OLED_CMD);   //设置行地址    0-127
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0x7f,OLED_DATA);
	
	OLED_Write_Data(0x5C,OLED_CMD);			// 使能MCU写入GRAM
	
	for(i=0;i<128;i++)
	{
	for(n=0;n<120;n++)
		{
			OLED_Write_Data(0,OLED_DATA); 
			OLED_Write_Data(0,OLED_DATA); 
		}
	}
}
/*void OLED_Test(void)//
{
			u8 i,n;
	    for(i=0;i<32;i++)
	{
		for(n=0;n<64;n++)
		{
			OLED_GRAM[i][n]=0x0f;
		}
	}
}
void OLED_ALL_ON(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_Write_Data (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write_Data(0xff,OLED_DATA); 
	} 
}
void OLED_Test(void)//测试ABCDEFGHIJKLMNOP   023456789
{
			u8 i,n,j;

		for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[0][j*8+n]=zimu[j][n];
				}
		}
				for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[1][j*8+n]=zimu[j][n+8];
				}
		}
						for(j=0;j<10;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[2][j*8+n]=DZ[j][n];
				}
		}
				   for(j=0;j<10;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[3][j*8+n]=DZ[j][n+8];
				}
		}
		for(i=4;i<8;i++)
	{
						for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
		{
	      OLED_GRAM[i][j*8+n]=0;
		}		
	}
	}
}
void OLED_BMP(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{     
		for(n=0;n<128;n++)
		{
			OLED_GRAM[i][n]= BMP[8*i+n/16][n%16];
			//OLED_GRAM[i][n]= BMP_1[i][n];

		}
	}  
}
void OLED_BMP_3(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{     
		for(n=0;n<128;n++)
		{
			OLED_GRAM[i][n]= BMP_3[8*i+n/16][n%16];
			//OLED_GRAM[i][n]= BMP_1[i][n];

		}
	}  
}
void OLED_XY(u8 X_OLED,u8 Y_OLED) //XY偏移量，|__对OLED_GRAM的内容进行移动
{  
	u8 i,n;		    
		OLED_Write_Data(0xD3,OLED_CMD);
		OLED_Write_Data(Y_OLED,OLED_CMD);
	for(i=0;i<8;i++)  
	{  
    OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00+X_OLED%16,OLED_CMD); //低地址   //128=8*16  所以分为8份  每份16列 
		OLED_Write_Data (0x10+X_OLED/16,OLED_CMD); //高地址    高地址负责选择份   低地址负责选择列
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA);
	}   
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 by,cy,temp=0;
	if(x>128||y>64)return;//超出范围了
	by=y%8;
	cy=y-1;
	switch(by)
	{
		case 1: temp=0x80;break;
	  case 2: temp=0x40;break;
		case 3: temp=0x20;break;
	  case 4: temp=0x10;break;
		case 5: temp=0x08;break;
	  case 6: temp=0x04;break;
		case 7: temp=0x02;break;
	  case 0: temp=0x01;break;
		default: break;
	}
	if(t)OLED_GRAM[7-cy/8][x-1]|=temp;
	else OLED_GRAM[7-cy/8][x-1]&=~temp;	    
}

void OLED_OUT_String(u8 x,u8 y,u8 value,u8 mode)
{
	u8 n;
	y=4-y;
	if(mode)//ASCII  模式
	{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[2*y][8*(x-1)+n]=ASCII[value][n];
				}	
				for(n=0;n<8;n++)
				{
	      OLED_GRAM[2*y+1][8*(x-1)+n]=ASCII[value][n+8];
				}	
	}
	else
	{
		    for(n=0;n<16;n++)
				{
	      OLED_GRAM[2*y][8*(x-1)+n]=String_ST[value][n];
				}	
				for(n=0;n<16;n++)
				{
	      OLED_GRAM[2*y+1][8*(x-1)+n]=String_ST[value][n+16];
				}	
	}
		
}
*/



