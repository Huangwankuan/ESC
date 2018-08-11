#include "OLED_SSD1322.h"
#include "FONT.h"
u8 OLED_GRAM_USER[64][32];
void SPI2_Init(void)
{
	
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��

RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����˫����
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����Ϊ��SPI
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ֡�ṹ
SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ʱ�����ո�
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�ڶ���ʱ����
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�ڲ�NSS�ź���SSIλ����
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//Ԥ��ƵֵΪ2  Ԥ��Ƶ32���ϻ���ֶϵ�
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����MSBλ��ʼ
SPI_InitStructure.SPI_CRCPolynomial = 7;//����CRCֵ����Ķ���ʽ
SPI_Init(SPI2,&SPI_InitStructure);//��ʼ������


SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
//////��λOLED
RES_OLED=1;
delay_ms(10);
RES_OLED=0;
delay_ms(50);
RES_OLED=1;
SPI2_WriteByte(0xff);//��������		 
 
GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_11);  //PB13/14/15����

}   

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_WriteByte(u8 SendData)
{		
	u8 feedback=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		feedback++;
			if(feedback>200)return 0;
		}
	SPI_I2S_SendData(SPI2, SendData); //ͨ������SPIx����һ������   SPIֻ�������ݲ���������
	return 1;				    				    
}

void OLED_Write_Data(u8 Write_Data_OLED,u8 decide)
{
	
	 NSS_SPI=0;
	if(decide==1)//1д����  0д����
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
//����OLED��ʾ
void OLED_Display_On(void)
{
	//OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	//OLED_Write_Data(0X14,OLED_CMD);  //ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
//	OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	//OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}
void OLED_Reset_Initial(void)
{
	
	RES_OLED=0;
	delay_ms(50);
	RES_OLED=1;

	OLED_Display_Off();
	
	OLED_Write_Data(0XFD,OLED_CMD); 		//����������
	OLED_Write_Data(0X12,OLED_DATA);   //0X12����  0X16����
	
	OLED_Write_Data(0XB3,OLED_CMD); 		//����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Write_Data(0X91,OLED_DATA);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	
	OLED_Write_Data(0XCA,OLED_CMD); 		//���ø������ű���
	OLED_Write_Data(63,OLED_DATA);   //2^6=64 
	
	OLED_Write_Data(0XA1,OLED_CMD);		 	//������ʾ��ֹ��  
	OLED_Write_Data(0x00,OLED_DATA);   //
	
	OLED_Write_Data(0XA2,OLED_CMD);		 	//������ʾƫ��
	OLED_Write_Data(0X00,OLED_DATA);   //
	
	OLED_Write_Data(0XA0,OLED_CMD);		 	//����ӳ��Ͷ�COMģʽ
	OLED_Write_Data(0X10,OLED_DATA);   //  
	OLED_Write_Data(0X01,OLED_DATA);   //
	
	OLED_Write_Data(0XB5,OLED_CMD);		 	// SET GPIO 
	OLED_Write_Data(0X00,OLED_DATA);   //DISABLE  ����
	
	OLED_Write_Data(0XAB,OLED_CMD);		 	//  VDDѡ��λ
	OLED_Write_Data(0X01,OLED_DATA);   //   ѡ���ⲿVDD
	
	OLED_Write_Data(0XB4,OLED_CMD);		 	//  ��ʾ����A  
	OLED_Write_Data(0XA0,OLED_DATA);   //   ʹ��VSL
	OLED_Write_Data(0XF8,OLED_DATA);   //    ��ǿ ��GS��ʾ����
	
	OLED_Write_Data(0XC1,OLED_CMD);		 	//  ���öԱȶ�  
	OLED_Write_Data(200,OLED_DATA);   //  0x00-0xff
	
	OLED_Write_Data(0XC7,OLED_CMD);		 	//   ��������
	OLED_Write_Data(200,OLED_DATA);   //  0xf0-0xff  
	
		OLED_Write_Data(0xB8,OLED_CMD); //  	 ���ûҶȱ�
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
 		OLED_Write_Data(0x00,OLED_CMD); 	// ʹ�ܻҶȱ�
	
	
	OLED_Write_Data(0XB1,OLED_CMD);		 	//  ������ĳ���
	OLED_Write_Data(0XE2,OLED_DATA);   //

	
	OLED_Write_Data(0XD1,OLED_CMD);		 	//    ��ʾ����B 
	OLED_Write_Data(0XA2,OLED_DATA);   //     ���� normal
	OLED_Write_Data(0X20,OLED_DATA);   //      
	
	OLED_Write_Data(0XBB,OLED_CMD);		 	//    ����Ԥ����ѹ
	OLED_Write_Data(0X1F,OLED_DATA);   //     0.6*VCC
	
	OLED_Write_Data(0XB6,OLED_CMD);		 	//     ���õڶ���Ԥ�������
	OLED_Write_Data(0X08,OLED_DATA);   //    

	
	OLED_Write_Data(0XBE,OLED_CMD);		 	//    ����V-com��ѹ  
	OLED_Write_Data(0X07,OLED_DATA);   //			 0.86*VCC
	
//	OLED_Write_Data(0XA4,OLED_CMD);		 	//   ȫ����ʾ�ر�  GS-Level 0  ��ͻҶȼ���
//	OLED_Write_Data(0XA5,OLED_CMD);		 	//   ȫ����ʾ��  GS-Level 15 ��߻Ҷȼ���
	OLED_Write_Data(0XA6,OLED_CMD);		 		//   ������ʾģʽ 
//	OLED_Write_Data(0XA7,OLED_CMD);		 	//   ������ʾģʽ   ���Datasheet
	
//OLED_Write_Data(0XA8,OLED_CMD);		 	//   ������ʾģʽ
	
	OLED_Write_Data(0XA9,OLED_CMD);		 	//    �˳�������ʾģʽ
	
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
	OLED_Write_Data (0x15,OLED_CMD);  //�����е�ַ     0-119  
	OLED_Write_Data(0x1C,OLED_DATA);
	OLED_Write_Data(0X5B  ,OLED_DATA);   // 0-64    64*4 =256
	
	OLED_Write_Data (0x75,OLED_CMD);   //�����е�ַ    0-127
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0X3F,OLED_DATA);
	
	OLED_Write_Data(0x5C,OLED_CMD);			// ʹ��MCUд��GRAM
	
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
void Refresh_1_4_Gram(u8 Gray_num)  //0-15  0��  15�Ҷ����
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
	OLED_Write_Data (0x15,OLED_CMD);  //�����е�ַ     0-119  
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0x77,OLED_DATA);
//	OLED_Write_Data(0x00,OLED_DATA);
	
	
	OLED_Write_Data (0x75,OLED_CMD);   //�����е�ַ    0-127
	OLED_Write_Data(0x00,OLED_DATA);
	OLED_Write_Data(0x7f,OLED_DATA);
	
	OLED_Write_Data(0x5C,OLED_CMD);			// ʹ��MCUд��GRAM
	
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
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(0xff,OLED_DATA); 
	} 
}
void OLED_Test(void)//����ABCDEFGHIJKLMNOP   023456789
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
void OLED_XY(u8 X_OLED,u8 Y_OLED) //XYƫ������|__��OLED_GRAM�����ݽ����ƶ�
{  
	u8 i,n;		    
		OLED_Write_Data(0xD3,OLED_CMD);
		OLED_Write_Data(Y_OLED,OLED_CMD);
	for(i=0;i<8;i++)  
	{  
    OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00+X_OLED%16,OLED_CMD); //�͵�ַ   //128=8*16  ���Է�Ϊ8��  ÿ��16�� 
		OLED_Write_Data (0x10+X_OLED/16,OLED_CMD); //�ߵ�ַ    �ߵ�ַ����ѡ���   �͵�ַ����ѡ����
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA);
	}   
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 by,cy,temp=0;
	if(x>128||y>64)return;//������Χ��
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
	if(mode)//ASCII  ģʽ
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



