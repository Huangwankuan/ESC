#include "OLED.h"
#include "FONT.h"
u8 OLED_GRAM[8][128];

void SPI2_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

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
	OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	OLED_Write_Data(0X14,OLED_CMD);  //ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}
void OLED_Reset_Initial(void)
{
	

	OLED_Display_Off();
	
	OLED_Write_Data(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Write_Data(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	
	OLED_Write_Data(0xA8,OLED_CMD); //��������·��
	OLED_Write_Data(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	
	OLED_Write_Data(0xD3,OLED_CMD); //������ʾƫ��
	OLED_Write_Data(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_Write_Data(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_Write_Data(0x8D,OLED_CMD); //��ɱ�����
	OLED_Write_Data(0x14,OLED_CMD); //bit2������/�ر�
	
	OLED_Write_Data(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_Write_Data(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;

	OLED_Write_Data(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127; //���Ҿ���        A0+C0���ᵹ����ʾ
	
	OLED_Write_Data(0xC8,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��   //���¾���
	
	OLED_Write_Data(0xDA,OLED_CMD); //����COMӲ����������
	OLED_Write_Data(0x10,OLED_CMD); //[5:4]����
		 
	OLED_Write_Data(0x81,OLED_CMD); //�Աȶ�����
	OLED_Write_Data(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	
	OLED_Write_Data(0xD9,OLED_CMD); //����Ԥ�������
	OLED_Write_Data(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	
	OLED_Write_Data(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_Write_Data(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write_Data(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	
	OLED_Write_Data(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ     A6������ʾ  A7������ʾ

	OLED_Display_On();
	OLED_Clean();
}

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA); 
	}   
}
void OLED_Clean(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(0,OLED_DATA); 
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




