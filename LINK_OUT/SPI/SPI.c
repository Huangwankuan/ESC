#include "SPI.h"

void SPI1_Initial(void)
{
	
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��

RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����˫����
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����Ϊ��SPI
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ֡�ṹ
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//ʱ�����ո�
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//�ڶ���ʱ����
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�ڲ�NSS�ź���SSIλ����816;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//Ԥ��ƵֵΪ8  ��Ϊ9MHZ
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����MSBλ��ʼ
SPI_InitStructure.SPI_CRCPolynomial = 7;//����CRCֵ����Ķ���ʽ
SPI_Init(SPI1,&SPI_InitStructure);//��ʼ������

SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
SPI1_Read_Write_Byte(0xff);//��������		 
 
GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);  //PB13/14/15����

}   

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����0  ����ERROR
u8 SPI1_Read_Write_Byte(u8 Data)
{		
	u8 feedback=0;			

	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		feedback++;
		if(feedback>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, Data); //ͨ������SPIx����һ������   SPIֻ�������ݲ���������
		
		
	feedback=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		feedback++;
		if(feedback>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����									    				    

	
	}
