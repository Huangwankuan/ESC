//****************************************************** 
//������OLED_GPIO_Initial();			GPIO����   
//			OLED_Reset_Initial();		OLED���� 
//			OLED_Refresh_Gram();		GRAMд��OLED
//Ŀ�ģ�OLED����
//˵����
//Maker��huangwankuan
//2017.2.6
//Version��V1.1
//All rights reserved
//******************************************************
#ifndef _OLED_h
#define _OLED_h

#include "sys.h"
#include "delay.h"   

#define OLED_CMD 1         //CMD ѡ������
#define OLED_DATA 0        //��������

#define ASCII_Mode 1
#define String_Mode 0

#define CMD_OLED PBout(14)  //SPI2����/�����ź�

#define RES_OLED PAout(8)  	//OLED��λ�ź�

#define NSS_SPI  PBout(12)  //SPI2Ƭѡ�ź�
  

void SPI2_Init(void);			 //��ʼ��SPI��
u8 SPI2_WriteByte(u8 SendData);
void OLED_Write_Data(u8 Write_Data_OLED,u8 decide);		 
void OLED_Display_On(void);   //����OLED��ʾ		
void OLED_Display_Off(void);  //�ر�OLED��ʾ
void OLED_Reset_Initial(void);//OLED��λ����������OLED�ĸ���Ĵ������ã�
void OLED_Refresh_Gram(void); //��GRAM�еĻ���д����OLED��
void OLED_Clean(void);//����   ���ݲ�д��GRAM����  ֱ��д��OLED
void OLED_ALL_ON(void);//ȫ��   ���ݲ�д��GRAM����  ֱ��д��OLED
void OLED_BMP(void);
void OLED_BMP_3(void);
void OLED_Test(void);
void OLED_XY(u8 X_OLED,u8 Y_OLED);//��λ����|__  
//GRAM��ֵ���ı� ��Ӳ��OLED���в���   �����1��1�����ʾ�����ƶ�һλ  �����ƶ�һλ
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_OUT_String(u8 x,u8 y,u8 value,u8 mode);
#endif