#include "CAP.h"
#include "MOTOR.h"
#include "UART.h"
u16 CCR2_Value[3];

extern u8 MOTOR_Satus;
u8 ERROR_Flag=0;

u8 ERROR_TIME=0;//用于检测堵转持续的时间  计数值代表测速采样的周期  设为100ms左右
u16 ERROR_BUF=0;//保存上次的测速值
void CAP_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//使能TIM2/TIM3/GPIOA时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB2Periph_GPIOA,ENABLE);
//使能三个捕获通道
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_1;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
//使能TIM2时基单元
TIM_TimeBaseStructure.TIM_Prescaler = 71;//预分频值 预分频值72  频率1MHZ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_Period = 65535;//在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化
	
	
//使能TIM2/捕获通道1
TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接连接
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每个边沿都捕获
TIM_ICInitStructure.TIM_ICFilter =4;//滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);
//使能TIM2/捕获通道2
TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接连接
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每个边沿都捕获
TIM_ICInitStructure.TIM_ICFilter =4;//滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);
//使能TIM2/捕获通道3
TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;//输入捕获上升沿
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接连接
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每个边沿都捕获
TIM_ICInitStructure.TIM_ICFilter =4;//滤波器
TIM_ICInit(TIM2,&TIM_ICInitStructure);

//使能TIM2_NVIC中断

NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//使能外部中断通道 2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 
/**
* TIM3用于测速使用 计数器每50MS更新一次  进入中断
*/
TIM_TimeBaseStructure.TIM_Prescaler = 720-1;//预分频值720  频率100K
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
TIM_TimeBaseStructure.TIM_Period = 2500-1;//重装债值  5000*0.01us = 50ms 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化TIM3时基单元

/**
* TIM3  NVIC中断优先级配置
*/
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;							//
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
NVIC_Init(&NVIC_InitStructure); 

//打开中断 启动定时器
TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3,ENABLE);
TIM_Cmd(TIM2, ENABLE);
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	
TIM_Cmd(TIM3, ENABLE);
}



u8  CAPTURE_STA=0x01;	//霍尔信号捕获状态
u32 SPEED_BUF=0;
volatile u16 SPEED_TIME=0;//单圈周期  单位us
volatile u16  SPEED_VALUE=0;//每分钟转速 RMP/MIN
u16 time=0;

void TIM3_IRQHandler(void)//TIM3中断服务函数   本时钟也用作堵转保护
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
					if((SPEED_VALUE==0)&&(ERROR_BUF==0))//上次状态和本次状态均为静止
					{
						 ERROR_TIME++;
						if(ERROR_TIME==8)
						{
							EXTI_GenerateSWInterrupt(EXTI_Line15);//产生中断线15软中断 定义中断线15为错误发生的外部中断
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
					EXTI_GenerateSWInterrupt(EXTI_Line4);//产生中断线4软中断
					CAPTURE_STA=0x01;//开始新一轮的采样
					TIM_SetCounter(TIM3,0);//第一次进入设置计数器值为0
			}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{

	  if((CAPTURE_STA&0x01)&&(time==0))//0表示可以开始新一轮的捕获
			{
						TIM_SetCounter(TIM2,0);//第一次进入设置计数器值为0
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
u8  CAPTURE_STA=0;	//霍尔信号捕获状态
//  0x80表示第一次信号被捕获  
//  0表示捕获过程已结束
//  0x01表示开始新一轮的捕获
u16	CAPTURE_VAL[3];	//捕获位点将会存储3个计数器的值
//每次捕获数值将会是一圈的周期
u16 *CAPTURE=CAPTURE_VAL;//指针

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
							for(i=0;i<1;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数1
			case 2:				
							for(i=0;i<2;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2/2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数2
			case 3:			
							for(i=0;i<3;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME*2/3;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数3
			default:
							SPEED_TIME=0;
							SPEED_VALUE=25;
							break;//其他次数已经出错
		 }
			for(i=0;i<3;i++)
			{
					CAPTURE_VAL[i]=0;
			}							
		 EXTI_GenerateSWInterrupt(EXTI_Line4);//产生中断线4软中断
		 CAPTURE_Counter=0;
		 TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
		 CAPTURE_STA=0x01;//开始新一轮的采样
		 CAPTURE=CAPTURE_VAL;//重新指向数组的首地址
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC1)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//开始捕获
				 {
					if(CAPTURE_STA&0x80)//捕获已经完成
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
					else //第一次高电平捕获位点已进入
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
///////////////////20170628修改前内容//////////////
/*

u8  CAPTURE_STA=0;	//霍尔信号捕获状态
//  0x80表示第一次信号被捕获  
//  0表示捕获过程已结束
//  0x01表示开始新一轮的捕获
u16	CAPTURE_VAL[3];	//捕获位点将会存储3个计数器的值
//每次捕获数值将会是一圈的周期
u16 *CAPTURE=CAPTURE_VAL;//指针

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
							for(i=0;i<1;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数1
			case 2:				
							for(i=0;i<2;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/2;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数2
			case 3:			
							for(i=0;i<3;i++)
							{
								SPEED_TIME+=CAPTURE_VAL[i];
							}
						   	SPEED_TIME=SPEED_TIME/3;
								SPEED_VALUE=1000*1000/SPEED_TIME;//60秒1000ms1000us
							break;//采样次数3
			default:		
							SPEED_TIME=20000;
							SPEED_VALUE=1000*1000/SPEED_TIME;
							break;//其他次数已经出错
		 }
			for(i=0;i<3;i++)
			{
					CAPTURE_VAL[i]=0;
			}							
		 EXTI_GenerateSWInterrupt(EXTI_Line4);//产生中断线4软中断
		 CAPTURE_Counter=0;
		 CAPTURE_STA=0x01;//开始新一轮的采样
		 CAPTURE=CAPTURE_VAL;//重新指向数组的首地址
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(void)
{
			if(TIM_GetITStatus(TIM2, TIM_IT_CC4)!=RESET)//
			{
				 if(CAPTURE_STA&0x01)//开始捕获
				 {
					if(CAPTURE_STA&0x80)//捕获已经完成
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
					else //第一次高电平捕获位点已进入
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



