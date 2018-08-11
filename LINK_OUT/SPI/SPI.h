//****************************************************** 
//函数：SPI2_Init(void);										//初始化SPI口 
//			SPI2_ReadWriteByte(u8 SendData);		//初始化SPI口
//目的：SPI2配置
//说明：
//Maker：huangwankuan
//2017.2.17
//Version：V1.0
//All rights reserved
//******************************************************

#ifndef __SPI_H
#define __SPI_H

//PB11 PB14 分别是OLED片选和OLED数据/命令控制位
#include "sys.h"
#include "stm32f10x.h"

void SPI1_Initial(void);			 //初始化SPI口
u8 SPI1_Read_Write_Byte(u8 Data);
		 
#endif

