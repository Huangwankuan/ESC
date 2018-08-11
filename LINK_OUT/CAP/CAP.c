#include "CAP.h"
#include "MOTOR.h"
#include "UART.h"
u16 CCR2_Value[3];

extern u8 MOTOR_Satus;
u8 ERROR_Flag=0;

u8 ERROR_TIME=0;//���ڼ���ת������ʱ��  ����ֵ������ٲ���������  ��Ϊ100ms����
u16 ERROR_BUF=0;//�����ϴεĲ���ֵ
void CAP_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//ʹ��TIM2/TIM3/GPIOAʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB2Periph_GPIOA,ENABLE);
//ʹ����������ͨ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_1;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
//ʹ��TIM2ʱ����Ԫ
TIM_TimeBaseStructure.TIM_Prescaler = 71;//Ԥ��Ƶֵ Ԥ��Ƶֵ72  Ƶ��1MHZ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_Period = 65535;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ��
	
	
//ʹ��TIM2/����ͨ��1
TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ֱ������
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ�����ض�����
TIM_ICInitStructure.TIM_ICFilter =4;//�˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);
//ʹ��TIM2/����ͨ��2
TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ֱ������
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ�����ض�����
TIM_ICInitStructure.TIM_ICFilter =4;//�˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);
//ʹ��TIM2/����ͨ��3
TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ֱ������
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ�����ض�����
TIM_ICInitStructure.TIM_ICFilter =4;//�˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);

//ʹ��TIM2_NVIC�ж�

NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//ʹ���ⲿ�ж�ͨ�� 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 
/**
* TIM3���ڲ���ʹ�� ������ÿ50MS����һ��  �����ж�
*/
TIM_TimeBaseStructure.TIM_Prescaler = 720-1;//Ԥ��Ƶֵ720  Ƶ��100K
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_Period = 2500-1;//��װծֵ  5000*0.01us = 50ms 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ��TIM3ʱ����Ԫ

/**
* TIM3  NVIC�ж����ȼ�����
*/
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;							//
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 

//���ж� ������ʱ��
TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3,ENABLE);
TIM_Cmd(TIM2, ENABLE);
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	
TIM_Cmd(TIM3, ENABLE);
}



u8  CAPTURE_STA=0x01;	//�����źŲ���״̬
u32 SPEED_BUF=0;
volatile u16 SPEED_TIME=0;//��Ȧ����  ��λus
volatile u16  SPEED_VALUE=0;//ÿ����ת�� RMP/MIN
u16 time=0;

void TIM3_IRQHandler(void)//TIM3�жϷ�����   ��ʱ��Ҳ������ת����
{
   int time_buf=time-1;

			if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
			{
					CAPTURE_STA=0;
					SPEED_TIME=0;
					SPEED_VALUE=0;
					if(time_buf==0)
					{
					SPEED_TIME=65535;
					SPEED_VALUE=0;
					}
					else
					{
								time_buf=(time_buf+time_buf*time_buf);
								SPEED_TIME=SPEED_BUF/time_buf;
								SPEED_TIME=SPEED_TIME*6;
								SPEED_VALUE=1000*1000/SPEED_TIME;
					}
					if((SPEED_VALUE==0)&&(ERROR_BUF==0))//�ϴ�״̬�ͱ���״̬��Ϊ��ֹ
					{
						 ERROR_TIME++;
						if(ERROR_TIME==8)
						{
							EXTI_GenerateSWInterrupt(EXTI_Line15);//�����ж���15���ж� �����ж���15Ϊ���������ⲿ�ж�
						//	ERROR_Flag=1;
							ERROR_TIME=0;
						}
					}
					else
					{
						ERROR_TIME=0;
					//	ERROR_Flag=0;
					}
					ERROR_BUF=SPEED_VALUE;
				//	printf("%d %d   aa\r\n",SPEED_TIME,SPEED_VALUE);
					SPEED_BUF=0;
					time_buf=0;
					time=0;
					EXTI_GenerateSWInterrupt(EXTI_Line4);//�����ж���4���ж�
					CAPTURE_STA=0x01;//��ʼ��һ�ֵĲ���
					TIM_SetCounter(TIM3,0);//��һ�ν������ü�����ֵΪ0
			}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{

	  if((CAPTURE_STA&0x01)&&(time==0))//0��ʾ���Կ�ʼ��һ�ֵĲ���
			{
						TIM_SetCounter(TIM2,0);//��һ�ν������ü�����ֵΪ0
					CAPTURE_STA|=0x80;
			}
				else
				{
								if((CAPTURE_STA&0x80)&&(time<=10))
						{
							
												if(TIM_GetITStatus(TIM2, TIM_IT_CC1)!=RESET)//
											{
															SPEED_BUF+=TIM_GetCapture1(TIM2);
											}
											else if(TIM_GetITStatus(TIM2, TIM_IT_CC2)!=RESET)
											{
															SPEED_BUF+=TIM_GetCapture2(TIM2);
											}
											else if(TIM_GetITStatus(TIM2, TIM_IT_CC3)!=RESET)
											{
															SPEED_BUF+=TIM_GetCapture3(TIM2);
											}
											else{}								
							}
			}
										if(time<=10)
				{
				time++;	
				}	

								TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
								TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
								TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
				if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)
				{ 
								TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				}
			//	printf("time:%d  SPEED_BUF:%d\r\n",time,SPEED_BUF);
}

/*
u8  CAPTURE_STA=0;	//�����źŲ���״̬
//  0x80��ʾ��һ���źű�����  
//  0��ʾ��������ѽ���
//  0x01��ʾ��ʼ��һ�ֵĲ���
u16	CAPTURE_VAL[3];	//����λ�㽫��洢3����������ֵ
//ÿ�β�����ֵ������һȦ������
u16 *CAPTURE=CAPTURE_VAL;//ָ��

u8  CAPTURE_Counter=0;	//һ�ֲ��񽫻��������

u16  SPEED_TIME=0;//��Ȧ����  ��λus

u16  SPEED_VALUE=0;//ÿ����ת�� RMP/MIN

void TIM3_IRQHandler(void)//TIM3�жϷ�����
{
	CAPTURE_STA=0;//ֹͣ����
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
	{
			int i;
			CAPTURE_STA=0;
			SPEED_TIME=0;
			SPEED_VALUE=0;
     switch(CAPTURE_Counter)
		 {
			case 1:
							for(i=0;i<1;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������1
			case 2:				
							for(i=0;i<2;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2/2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������2
			case 3:			
							for(i=0;i<3;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2/3;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������3
			default:
							SPEED_TIME=0;
							SPEED_VALUE=25;
							break;//���������Ѿ�����
		 }
			for(i=0;i<3;i++)
			{
					CAPTURE_VAL[i]=0;
			}							
		 EXTI_GenerateSWInterrupt(EXTI_Line4);//�����ж���4���ж�
		 CAPTURE_Counter=0;
		 TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
		 CAPTURE_STA=0x01;//��ʼ��һ�ֵĲ���
		 CAPTURE=CAPTURE_VAL;//����ָ��������׵�ַ
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC1)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
					if(CAPTURE_STA&0x80)//�����Ѿ����
					{
							*CAPTURE++=TIM_GetCapture1(TIM2);//	
							CAPTURE_Counter++;
							if(CAPTURE_Counter>=3)//
							{
							   CAPTURE_STA=0;
							}
							else
							{
								 CAPTURE_STA=0x01;  //
								 TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
							}	

					}
					else //��һ�θߵ�ƽ����λ���ѽ���
					{
							TIM_SetCounter(TIM2,0);
						   TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);
							CAPTURE_STA|=0x80;
					}
				}
	}
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);//
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}




*/
///////////////////20170628�޸�ǰ����//////////////
/*

u8  CAPTURE_STA=0;	//�����źŲ���״̬
//  0x80��ʾ��һ���źű�����  
//  0��ʾ��������ѽ���
//  0x01��ʾ��ʼ��һ�ֵĲ���
u16	CAPTURE_VAL[3];	//����λ�㽫��洢3����������ֵ
//ÿ�β�����ֵ������һȦ������
u16 *CAPTURE=CAPTURE_VAL;//ָ��

u8  CAPTURE_Counter=0;	//һ�ֲ��񽫻��������

u16  SPEED_TIME=0;//��Ȧ����  ��λus

u16  SPEED_VALUE=0;//ÿ����ת�� RMP/MIN

void TIM3_IRQHandler(void)//TIM3�жϷ�����
{
	CAPTURE_STA=0;//ֹͣ����
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
	{
			int i;
			CAPTURE_STA=0;
			SPEED_TIME=0;
			SPEED_VALUE=0;
     switch(CAPTURE_Counter)
		 {
			case 1:
							for(i=0;i<1;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������1
			case 2:				
							for(i=0;i<2;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������2
			case 3:			
							for(i=0;i<3;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/3;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������3
			default:		
							SPEED_TIME=20000;
							SPEED_VALUE=1000*1000/SPEED_TIME;
							break;//���������Ѿ�����
		 }
			for(i=0;i<3;i++)
			{
					CAPTURE_VAL[i]=0;
			}							
		 EXTI_GenerateSWInterrupt(EXTI_Line4);//�����ж���4���ж�
		 CAPTURE_Counter=0;
		 CAPTURE_STA=0x01;//��ʼ��һ�ֵĲ���
		 CAPTURE=CAPTURE_VAL;//����ָ��������׵�ַ
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC4)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
					if(CAPTURE_STA&0x80)//�����Ѿ����
					{
							*CAPTURE++=TIM_GetCapture4(TIM2);//	
							CAPTURE_Counter++;
							if(CAPTURE_Counter>=3)//
							{
							   CAPTURE_STA=0;
							}
							else
							{
								 CAPTURE_STA=0x01;  //
							}	

					}
					else //��һ�θߵ�ƽ����λ���ѽ���
					{
							TIM_SetCounter(TIM2,0);
							CAPTURE_STA|=0x80;
					}
				}
	}
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);//
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
*/



