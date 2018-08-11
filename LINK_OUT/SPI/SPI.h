//****************************************************** 
//������SPI2_Init(void);										//��ʼ��SPI�� 
//			SPI2_ReadWriteByte(u8 SendData);		//��ʼ��SPI��
//Ŀ�ģ�SPI2����
//˵����
//Maker��huangwankuan
//2017.2.17
//Version��V1.0
//All rights reserved
//******************************************************

#ifndef __SPI_H
#define __SPI_H

//PB11 PB14 �ֱ���OLEDƬѡ��OLED����/�������λ
#include "sys.h"
#include "stm32f10x.h"

void SPI1_Initial(void);			 //��ʼ��SPI��
u8 SPI1_Read_Write_Byte(u8 Data);
		 
#endif

