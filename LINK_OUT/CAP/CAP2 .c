#include "CAP.h"
#include "MOTOR.h"
u16 CCR2_Value[3];
extern u8 MOTOR_Satus;
void CAP_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

TIM_TimeBaseStructure.TIM_Prescaler = 71;//Ԥ��Ƶֵ Ԥ��Ƶֵ72  Ƶ��1MHZ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_Period = 65535;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ��


TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIMͨ��2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//����1,2,3��4�ֱ���IC1,2,3��4����
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ̽�⵽1�����ض�����һ��
TIM_ICInitStructure.TIM_ICFilter =3;//���벶���˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);//��ʼ��


TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIMͨ��2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//����1,2,3��4�ֱ���IC1,2,3��4����
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ̽�⵽1�����ض�����һ��
TIM_ICInitStructure.TIM_ICFilter =3;//���벶���˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);//��ʼ��


TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIMͨ��2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//���벶��������
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//����1,2,3��4�ֱ���IC1,2,3��4����
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//ÿ̽�⵽1�����ض�����һ��
TIM_ICInitStructure.TIM_ICFilter =3;//���벶���˲���
TIM_ICInit(TIM2,&TIM_ICInitStructure);//��ʼ��


NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//ʹ���ⲿ�ж�ͨ�� 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	

TIM_Cmd(TIM2, ENABLE);//ʹ�ܻ���ʧ��TIMx����

/**
* TIM3���ڲ���ʹ�� ������ÿ50MS����һ��  �����ж�
*/
TIM_TimeBaseStructure.TIM_Prescaler = 720-1;//Ԥ��Ƶֵ720  Ƶ��100K
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
TIM_TimeBaseStructure.TIM_Period = 5000-1;//��װծֵ  5000*0.01us = 50ms 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ��TIM3ʱ����Ԫ

/**
* TIM3  NVIC�ж����ȼ�����
*/
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;							//
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//��ռ���ȼ�
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
NVIC_Init(&NVIC_InitStructure); 

TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//����TIM3�����ж�

TIM_Cmd(TIM3, ENABLE);										//ʹ��TIM3

}


u8  CAPTURE_STA=0;	//�����źŲ���״̬
//  0x80��ʾ��һ���źű�����  
//  0x40��ʾ�ڶ����źű�����
//  0x20��ʾ�������źű�����
//  0x40��ʾ���Ĵ��źű�����
//  0��ʾ��������ѽ���
//  0x01��ʾ��ʼ��һ�ֵĲ���
//  0x02��ʾ�����Ѿ�ת�����
u16	CAPTURE_VAL[9];	//����λ�㽫��洢9����������ֵ
//һ�� *  *  *
//���� *  *  *
//���� *  *  *

u16 *CAPTURE=CAPTURE_VAL;

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
							for(i=0;i<3;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/2;
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������1
			case 2:				
							for(i=0;i<6;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/4;
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������2
			case 3:			
							for(i=0;i<9;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/6;
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60��1000ms1000us
							break;//��������3
			default:		
							SPEED_TIME=22222;
							SPEED_VALUE=22222;
							break;//���������Ѿ�����
		 }
			for(i=0;i<9;i++)
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

    if(MOTOR_Satus==MOTOR_CCW)
		{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC4)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
					if(CAPTURE_STA&0x80)//���в����Ѿ����
					{
							*CAPTURE++=TIM_GetCapture4(TIM2);//	
							CAPTURE_Counter++;
							if(CAPTURE_Counter>=3)//��������ﵽ���� �ر�
							{
							   CAPTURE_STA=0;
							}
							else
							{
								 CAPTURE_STA=0x01;  //��δ�ﵽ����  ����
								 
							}	

					}
					else //��һ�θߵ�ƽ����λ���ѽ���
					{
							TIM_SetCounter(TIM2,0);
							CAPTURE_STA|=0x40;
					}
				}
			}
			
			if(TIM_GetITStatus(TIM2, TIM_IT_CC3)!=RESET)//
			{
				if(CAPTURE_STA&0x01)//��ʼ����
				{
					if(CAPTURE_STA&0x40)//
					{
							*CAPTURE++=TIM_GetCapture3(TIM2);
							 CAPTURE_STA|=0x20;
					}
				}

			}
			if(TIM_GetITStatus(TIM2, TIM_IT_CC2)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
						if(CAPTURE_STA&0x20)//
						{
							*CAPTURE++=TIM_GetCapture2(TIM2);
								 CAPTURE_STA|=0x80;
						}
				 }

			}
	}
	
/*	    if(MOTOR_Satus==MOTOR_CW)
		{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC2)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
					if(CAPTURE_STA&0x80)//���в����Ѿ����
					{
							CAPTURE_VAL[0]=TIM_GetCapture2(TIM2);//	
							CAPTURE_STA=0;							
							EXTI_GenerateSWInterrupt(EXTI_Line4);//
					}
					else //��һ�θߵ�ƽ����λ���ѽ���
					{
								CAPTURE_VAL[0]=0;
								CAPTURE_VAL[1]=0;
								CAPTURE_VAL[2]=0;							
							TIM_SetCounter(TIM2,0);
							CAPTURE_STA|=0x40;
					}
				}
			}
			
			if(TIM_GetITStatus(TIM2, TIM_IT_CC3)!=RESET)//
			{
				if(CAPTURE_STA&0x01)//��ʼ����
				{
					if(CAPTURE_STA&0x40)//
					{
							 CAPTURE_VAL[1]=TIM_GetCapture3(TIM2);
							 CAPTURE_STA|=0x20;
					}
				}

			}
			if(TIM_GetITStatus(TIM2, TIM_IT_CC4)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//��ʼ����
				 {
						if(CAPTURE_STA&0x20)//
						{
								 CAPTURE_VAL[2]=TIM_GetCapture4(TIM2);
								 CAPTURE_STA|=0x80;
						}
				 }

			}
	}
	
	
		  if((MOTOR_Satus==MOTOR_STOP)||(MOTOR_Satus==MOTOR_STANDBY))
		{
				CAPTURE_VAL[0]=0;//	
				CAPTURE_VAL[0]=0;//	
				CAPTURE_VAL[0]=0;//	
				CAPTURE_STA=0;							
		}
	*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);//
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);//
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);//
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
