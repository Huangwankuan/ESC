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

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);//使能或者失能APB1外设时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx寄存器

TIM_TimeBaseStructure.TIM_Prescaler = 71;//预分频值 预分频值72  频率1MHZ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_Period = 65535;//在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化


TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//TIM通道2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//输入1,2,3或4分别与IC1,2,3或4相连
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每探测到1个边沿都捕获一次
TIM_ICInitStructure.TIM_ICFilter =3;//输入捕获滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);//初始化


TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;//TIM通道2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//输入1,2,3或4分别与IC1,2,3或4相连
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每探测到1个边沿都捕获一次
TIM_ICInitStructure.TIM_ICFilter =3;//输入捕获滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);//初始化


TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//TIM通道2
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//输入1,2,3或4分别与IC1,2,3或4相连
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每探测到1个边沿都捕获一次
TIM_ICInitStructure.TIM_ICFilter =3;//输入捕获滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);//初始化


NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//使能外部中断通道 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器

TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	

TIM_Cmd(TIM2, ENABLE);//使能或者失能TIMx外设

/**
* TIM3用于测速使用 计数器每50MS更新一次  进入中断
*/
TIM_TimeBaseStructure.TIM_Prescaler = 720-1;//预分频值720  频率100K
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_Period = 5000-1;//重装债值  5000*0.01us = 50ms 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化TIM3时基单元

/**
* TIM3  NVIC中断优先级配置
*/
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;							//
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 

TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//允许TIM3更新中断

TIM_Cmd(TIM3, ENABLE);										//使能TIM3

}


u8  CAPTURE_STA=0;	//霍尔信号捕获状态
//  0x80表示第一次信号被捕获  
//  0x40表示第二次信号被捕获
//  0x20表示第三次信号被捕获
//  0x40表示第四次信号被捕获
//  0表示捕获过程已结束
//  0x01表示开始新一轮的捕获
//  0x02表示数据已经转换完成
u16	CAPTURE_VAL[9];	//捕获位点将会存储9个计数器的值
//一次 *  *  *
//二次 *  *  *
//三次 *  *  *

u16 *CAPTURE=CAPTURE_VAL;

u8  CAPTURE_Counter=0;	//一轮捕获将会持续三次

u16  SPEED_TIME=0;//单圈周期  单位us

u16  SPEED_VALUE=0;//每分钟转速 RMP/MIN

void TIM3_IRQHandler(void)//TIM3中断服务函数
{
	CAPTURE_STA=0;//停止采样
	
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
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数1
			case 2:				
							for(i=0;i<6;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/4;
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数2
			case 3:			
							for(i=0;i<9;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/6;
								SPEED_VALUE=60*1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数3
			default:		
							SPEED_TIME=22222;
							SPEED_VALUE=22222;
							break;//其他次数已经出错
		 }
			for(i=0;i<9;i++)
			{
					CAPTURE_VAL[i]=0;
			}							
		 EXTI_GenerateSWInterrupt(EXTI_Line4);//进行中断线4软中断
		 CAPTURE_Counter=0;
		 CAPTURE_STA=0x01;//开始新一轮的采样
		 CAPTURE=CAPTURE_VAL;//重新指向数组的首地址
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{

    if(MOTOR_Satus==MOTOR_CCW)
		{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC4)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//开始捕获
				 {
					if(CAPTURE_STA&0x80)//所有捕获已经完成
					{
							*CAPTURE++=TIM_GetCapture4(TIM2);//	
							CAPTURE_Counter++;
							if(CAPTURE_Counter>=3)//如果采样达到三次 关闭
							{
							   CAPTURE_STA=0;
							}
							else
							{
								 CAPTURE_STA=0x01;  //尚未达到三次  继续
								 
							}	

					}
					else //第一次高电平捕获位点已进入
					{
							TIM_SetCounter(TIM2,0);
							CAPTURE_STA|=0x40;
					}
				}
			}
			
			if(TIM_GetITStatus(TIM2, TIM_IT_CC3)!=RESET)//
			{
				if(CAPTURE_STA&0x01)//开始捕获
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
				 if(CAPTURE_STA&0x01)//开始捕获
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
				 if(CAPTURE_STA&0x01)//开始捕获
				 {
					if(CAPTURE_STA&0x80)//所有捕获已经完成
					{
							CAPTURE_VAL[0]=TIM_GetCapture2(TIM2);//	
							CAPTURE_STA=0;							
							EXTI_GenerateSWInterrupt(EXTI_Line4);//
					}
					else //第一次高电平捕获位点已进入
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
				if(CAPTURE_STA&0x01)//开始捕获
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
				 if(CAPTURE_STA&0x01)//开始捕获
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
