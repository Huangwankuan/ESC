#include "ADC.h"

#define ADC_Channel 3
#define ADC_Sample_Time 50

u16 ADC_BUF [ADC_Sample_Time][ADC_Channel];

u16 ADC_VALUE [3];

void ADC_Initial(void)
{
ADC_InitTypeDef ADC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能或者失能APB2外设时钟
	
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
	
RCC_ADCCLKConfig(RCC_PCLK2_Div8);//设置ADC时钟（ADCCLK）
		
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//
GPIO_Init(GPIOA, &GPIO_InitStructure);//
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//
GPIO_Init(GPIOB, &GPIO_InitStructure);//
	
	
ADC_DeInit(ADC1);//将外设ADCx的全部寄存器重设为缺省值

ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
ADC_InitStructure.ADC_ScanConvMode = ENABLE;//单次转换
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
ADC_InitStructure.ADC_NbrOfChannel = ADC_Channel;//
	
ADC_Init(ADC1,&ADC_InitStructure);//初始化配置

	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 3, ADC_SampleTime_239Cycles5);
	
	//ADC_ExternalTrigConvCmd(ADC1,DISABLE);
	//ADC_ExternalTrigInjectedConvCmd(ADC1,DISABLE);
	
	ADC_DMACmd(ADC1, ENABLE); 

	ADC_Cmd(ADC1,ENABLE);//使能或者失能指定的ADC
	
	
			/* Enable ADC1 reset calibration register */   
 ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
 while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
 ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
 while(ADC_GetCalibrationStatus(ADC1));
	
		DMA_DeInit(DMA1_Channel1);//
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	DMA_InitStructure.DMA_BufferSize = ADC_Channel*ADC_Sample_Time;               
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	


  /* Configure and enable ADC interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
		
	 DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

		DMA_Cmd(DMA1_Channel1, ENABLE);  //
		
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);//
}


void DMA1_Channel1_IRQHandler(void)
{
 u8 i=0;
	u32 BUF1=0;
	u32 BUF2=0;
	u32 BUF3=0;
	for(i=0;i<ADC_Sample_Time;i++)
	{
			BUF1+=ADC_BUF[i][0];
			BUF2+=ADC_BUF[i][1];
			BUF3+=ADC_BUF[i][2];
	}
	ADC_VALUE[0]=BUF1/ADC_Sample_Time;
	ADC_VALUE[1]=BUF2/ADC_Sample_Time;
	ADC_VALUE[2]=BUF3/ADC_Sample_Time;	
	DMA_ClearITPendingBit(DMA1_IT_TC1);
}
