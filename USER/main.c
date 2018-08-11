#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#include "ADC.h"
#include "LED.h"
#include "TIM.h"
#include "MOTOR.h"
#include "EXTI.h"
#include "KEY.h"
#include "UART.h"
#include "CAN.h"
#include "CAP.h"
#include "PID.h"
#include "24C02.h" 

#define True 0
#define False 1

#define SPEED_MIN 10

//a NEW LINE

u8 KEY_STA=0;//KEY_STA 按键工作模  最低位0为运行模式  1为设置NUM_ESC模式

u8 RCC_Initial(void);

u8 G_Data=0;//最低位为霍尔信号1 

u8 CAN_Send_DATA[8];

u16 Current=0;
u16 ADC_GET=0;

extern u16 A_Value;//

extern u8 CAN_Read_DATA[8];

extern const u8 G_IN[6];

extern u8 MOTOR_Satus;

extern  u8 MOTOR_ON_Satus;
extern u8 MOTOR_Satus_Last;
extern u16 SPEED_VALUE;
extern u16 SPEED_TIME;

extern u8 ERROR_Flag;

MOTOR_Status_TypeDef MOTOR_Handler;


extern u16 ADC_VALUE [3];

u16 SPEED_LAST=0;


u16 MOTOR_PWM=0;


u8 Num_ESC;
u8 CW_CCW=0;

PID_Value PID1;
//float PID1_KP=0.10;
//float PID1_T=0.6;
//float PID1_Ti=0.2;


//Ti越小  震荡越明显  同时响应会更快
//粗略值：
//P值在1-2之间
//float PID1_KP=1.2;
//float PID1_KI=0.2;
//float PID1_KD=0;
//float K_PWM=1.5;


float PID1_KP=0.8;
float PID1_KI=0.2;
float PID1_KD=0;
float K_PWM=1.5;

float PID1_KP_S=0.1;
float PID1_KI_S=0.08;
float PID1_KD_S=0;

#define CW 2
#define CCW 3
u8 Current_FB(u16 REF,u16 IN);
u8 PID_SAFE_Correct(PID_Value PID);
void MOTOR_PWM_SAFE(u16 PWM);//用于在出现危险时把PWM调为0；
u8 SET_NUM_ESC(void);
void MOTOR_RESET(void);


void MOTOR_Dir(void)
{
	MOTOR_Handler.direction=AT24CXX_ReadOneByte(1);	
	if(MOTOR_Handler.direction>1)
	{
		MOTOR_Handler.direction=1;
	}//有效性判断
}

u8 MOTOR_Set_Status(u8 Status)
{
	 u8 R_Status;
	if(MOTOR_Handler.direction==0)
	{
		R_Status=Status;
	}
	else
	{
		if(Status==MOTOR_CW)R_Status=MOTOR_CCW;
		else R_Status=MOTOR_CW;
	}
 return R_Status;
}

int main(void)
{
int i=0;
	
PID_Initial(PID1_KP,PID1_KI,PID1_KD,&PID1);

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级

RCC_Initial();
	
delay_init();
	
LED_Initial();
	
TIM_PWM_Initial(299,30,0);
	
ADC_Initial();
	
MOTOR_GPIO_Initial();
	
MOTOR_HALL_Initial();
	
MOTOR_Phase(0);//待机状态

EXTI_MOTOR_Phas_Initial();//霍尔信号外部中断初始化

KEY_Initial();

AT24CXX_Init();

UART1_Initial(115200);

MOTOR_ON_Satus=MOTOR_OFF;	

SET_NUM_ESC();

CAN1_Initial(Num_ESC);

MOTOR_RESET();

for(i=0;i<8;i++)
{
CAN_Send_DATA[i]=0x00;
}

CAP_Initial();


	LED_G=OFF;
	LED_R=OFF;
	LED_B=OFF;
//AT24CXX_WriteOneByte(254,0x04);



//TIM_SetCompare2(TIM1,100);
/*MOTOR_ON_Satus=MOTOR_OFF;	
MOTOR_Satus=MOTOR_STANDBY;*/
//MOTOR_UP(MOTOR_Satus);

MOTOR_ON_Satus=MOTOR_OFF;	

MOTOR_Handler.direction=0;
MOTOR_Dir();

MOTOR_RESET();

//MOTOR_ON_Satus=MOTOR_ON;	
//MOTOR_Satus=MOTOR_CW;
//MOTOR_UP(MOTOR_Satus);
//PID1.PID_SET=35;

//TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断

while(1)
	{

		delay_ms(5);
		//printf("NUM:%d\r\n",AT24CXX_ReadOneByte(0));
//  printf("ADC1_Value1:%d   ADC1_Value2:%d  ADC1_Value3:%d  FLAG:%d  I:%d\r\n",ADC_VALUE[0],ADC_VALUE[1],
//		ADC_VALUE[2],ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC),Current_FB(ADC_VALUE[1],ADC_VALUE[2]));
		if(KEY_1==1)
		{
			delay_ms(5);
			if(KEY_1==1)
			{
						LED_G=~LED_G;
			}
			while(KEY_1==1);
		}
		
				if(KEY_2==1)
		{
			delay_ms(5);
			if(KEY_2==1)
			{
				LED_B=~LED_B;
			}
			while(KEY_2==1);
				}
	
				
}
}


u8 SET_NUM_ESC(void)
{
	if(KEY_FB()==2)//KEY1按下 KEY2未按下
{
	LED_G=OFF;
	LED_R=ON;
	LED_B=OFF;
	while(KEY_FB()==2)
	{
		LED_B=ON;
		delay_ms(100);
		LED_B=OFF;
		delay_ms(100);
	}
	KEY_STA=0x01;
	LED_G=OFF;
	LED_R=ON;
	LED_B=OFF;
	
	Num_ESC=AT24CXX_ReadOneByte(0);
	if((Num_ESC>4)||(Num_ESC==0))//1 2 3 4
	{
		Num_ESC=1;
		AT24CXX_WriteOneByte(0,Num_ESC);
	}
	delay_ms(50);
	while(KEY_STA==1)
	{
		if(Num_ESC==1)
		{
	
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			delay_ms(1000);
		}
				if(Num_ESC==2)
		{
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			delay_ms(1000);
		}
				if(Num_ESC==3)
		{
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			delay_ms(1000);
		}
				if(Num_ESC==4)
		{
					
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);
			
			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);

			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);

			LED_G=ON;
			delay_ms(150);
			LED_G=OFF;
			delay_ms(150);		
			
			delay_ms(1000);
		}
	}
		AT24CXX_WriteOneByte(0,Num_ESC);
}
 Num_ESC=AT24CXX_ReadOneByte(0);
	KEY_STA=0;
 return Num_ESC;
}

u8 Current_FB(u16 REF,u16 IN)//电流初略计算，计算的值为三路相电流之和，误差±0.5A；
{
	 int buf;
	 u8 I_Fb;
	 buf=IN-REF;
	 if(buf<=0)I_Fb=5;
	
	 else
		{
				I_Fb=(u8)((double)(IN-REF)*825/4096+5);
		}
		return I_Fb;
}


u8 PID_SAFE_Correct(PID_Value PID)
{
	if(PID.PID_SET<=SPEED_MIN)
	PID.PID_SET=SPEED_MIN;
	return 	True;
}

void MOTOR_RESET(void)//用于在危险后把所有相关设置复位
{
	MOTOR_ON_Satus=MOTOR_OFF;
	TIM_SetCompare2(TIM1,0);
	
	MOTOR_Satus=MOTOR_STANDBY;
 MOTOR_Phase(0);//
	
	
	PID1.SPEED_Now=0;
	SPEED_LAST=0;
	PID1.Error_Last=0;
	PID1.SPEED_Now=0;
	PID1.PID_SET=0;
	PID1.PID_OUT=0;
	
	
}

void MOTOR_PWM_SAFE(u16 PWM)//用于在出现危险时把PWM调为0；
{
	  if(MOTOR_ON_Satus==MOTOR_OFF)
			{	
			TIM_SetCompare2(TIM1,0);
			}
			else
			{
	 		TIM_SetCompare2(TIM1,PWM);
			}
}

void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line6)!=RESET)//KEY1被按下
	{
		  if(KEY_STA==1)
				{
						KEY_STA=0;
				}
				if(MOTOR_Handler.direction==0)
				{
					MOTOR_Handler.direction=1;
					AT24CXX_WriteOneByte(1,MOTOR_Handler.direction);
				}
				else
				{
					MOTOR_Handler.direction=0;
					AT24CXX_WriteOneByte(1,MOTOR_Handler.direction);
				}

				EXTI_ClearITPendingBit(EXTI_Line6);
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		MOTOR_RESET();
		//printf("ERROR\r\n");
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
			if(EXTI_GetITStatus(EXTI_Line11)!=RESET)//KEY2被按下
	{
				  if(KEY_STA==1)
				{
					Num_ESC++;
						if(Num_ESC==5)
						{
							Num_ESC=1;
						}
				}
				EXTI_ClearITPendingBit(EXTI_Line11);
	}
}




void EXTI4_IRQHandler(void)//每次捕获数据完成将会产生软中断
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		
		if((SPEED_VALUE==0)&&(MOTOR_ON_Satus==MOTOR_ON))
		{
					MOTOR_Satus=MOTOR_Set_Status(CAN_Read_DATA[2]);
					MOTOR_UP(MOTOR_Satus);
		}
		
		
	 PID_SAFE_Correct(PID1);
		
		PID1.SPEED_Now=SPEED_VALUE*0.7+SPEED_LAST*0.3;
	//	PID1.SPEED_Now=SPEED_VALUE;
		SPEED_LAST=PID1.SPEED_Now;
		
	if(PID1.SPEED_Now<=50)
	{
		PID1.KP=PID1_KP_S;
		PID1.KI=PID1_KI_S;
	}
	else
	{
		PID1.KP=PID1_KP;
		PID1.KI=PID1_KI;
	}
	if((MOTOR_Satus==MOTOR_CCW)||(MOTOR_Satus==MOTOR_CW))//保证只在运行情况下打开PID
	{
		PID_Handle(&PID1);
	}
	  MOTOR_PWM=PID1.PID_OUT/K_PWM;
		//MOTOR_PWM=30;
	
		if(MOTOR_PWM>=300)
		{
				MOTOR_PWM=300;
		}
//		else if(MOTOR_PWM<=20)
//		{
//				MOTOR_PWM=20;
//		}
//		else
//		{
//		
//		}

		MOTOR_PWM_SAFE(MOTOR_PWM);

		CAN_Send_DATA[0]=Num_ESC;
		CAN_Send_DATA[1]=MOTOR_ON_Satus;
		
		CAN_Send_DATA[2]=PID1.PID_SET>>8;
		CAN_Send_DATA[3]=PID1.PID_SET&0x00ff;
	//	
		CAN_Send_DATA[4]=SPEED_VALUE>>8;
		CAN_Send_DATA[5]=SPEED_VALUE&0x00ff;
		
		CAN_Send_DATA[6]=MOTOR_PWM>>8;
		CAN_Send_DATA[7]=MOTOR_PWM&0x00ff;
		
	 // CAN_SEND(CAN_Send_DATA,8);
  printf("%d %d %d %d %d %d %d %d\r\n",SPEED_TIME,SPEED_VALUE,PID1.PID_OUT,PID1.PID_SET,MOTOR_PWM,MOTOR_ON_Satus,MOTOR_Satus,Current_FB(ADC_VALUE[1],ADC_VALUE[2]));
	 //printf("%d %d %d %d %d %d\r\n",SPEED_TIME,SPEED_VALUE,PID1.PID_OUT,PID1.PID_SET,MOTOR_PWM, CAN_SEND(0x210+Num_ESC,CAN_Send_DATA,8));
 	EXTI_ClearITPendingBit(EXTI_Line4);			
 }

}




void USB_LP_CAN1_RX0_IRQHandler(void)
{
	int i=0;
CanRxMsg RxMessage;
if(CAN_GetITStatus(CAN1,CAN_IT_FMP0 ))//检查指定的CAN中断发生与否
{
   CAN_Receive( CAN1 , CAN_FIFO0 , &RxMessage);
	  for(i=0;i<8;i++)
	  if(RxMessage.StdId==(0x200+Num_ESC))
			{
			CAN_Read_DATA[i]=RxMessage.Data[i];
			}
//			printf("%X %X %X %X %X %X %X %X %X\r\n",CAN_Read_DATA[0],CAN_Read_DATA[1],CAN_Read_DATA[2],
//			CAN_Read_DATA[3],CAN_Read_DATA[4],CAN_Read_DATA[5],CAN_Read_DATA[6],CAN_Read_DATA[7],RxMessage.StdId);
			

			if(CAN_Read_DATA[2]==MOTOR_STANDBY)
			 {
				  	if(CAN_Read_DATA[1]>5)
							{
       MOTOR_RESET();
							MOTOR_ON_Satus=MOTOR_ON;	
							MOTOR_Satus=MOTOR_Set_Status(CAN_Read_DATA[2]);
							MOTOR_UP(MOTOR_Satus);
							}
							else
							{
							MOTOR_RESET();
							}
			 }
	else if (CAN_Read_DATA[2]==MOTOR_STOP)
			 {
  	    MOTOR_RESET();
							MOTOR_ON_Satus=MOTOR_ON;	
							MOTOR_Satus=MOTOR_Set_Status(CAN_Read_DATA[2]);
							MOTOR_UP(MOTOR_Satus);
			 }
	else 
			 {
							if(MOTOR_Satus_Last==CAN_Read_DATA[2])
							{
										MOTOR_ON_Satus=MOTOR_ON;	
							   MOTOR_Satus=MOTOR_Set_Status(CAN_Read_DATA[2]);
       }			
       else
							{
						 MOTOR_RESET();
					 	MOTOR_ON_Satus=MOTOR_ON;	
							MOTOR_Satus=MOTOR_Set_Status(CAN_Read_DATA[2]);
							MOTOR_UP(MOTOR_Satus);
							}						
			 }
				
			PID1.PID_SET=(CAN_Read_DATA[0]<<8)+CAN_Read_DATA[1];
				
	 	MOTOR_Satus_Last=CAN_Read_DATA[2];
	  CAN_FIFORelease(CAN1,CAN_FIFO0);
}
}


void USB_HP_CAN1_TX_IRQHandler(void)
{
		if(CAN_GetITStatus(CAN1,CAN_IT_TME ))//
		{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME );//
		}
}


void EXTI2_IRQHandler(void)
{
 if(MOTOR_Satus==MOTOR_STANDBY)
			 {
			     MOTOR_ON_Satus=MOTOR_OFF;
			 			 MOTOR_Phase(0);//
			 }
	else if (MOTOR_Satus==MOTOR_CW)
			 {
				if(MOTOR_Get_G()==G_IN[1])//霍尔状态2
						{
								MOTOR_Phase(6);//
								LED_B=~LED_B;
						}
						if(MOTOR_Get_G()==G_IN[4])//霍尔状态5
						{
								MOTOR_Phase(3);//
								LED_B=~LED_B;
						}	
				MOTOR_ON_Satus=MOTOR_ON;	
			 }
	else if (MOTOR_Satus==MOTOR_CCW)
			 {

					if(MOTOR_Get_G()==G_IN[2])//霍尔状态3
					{
							MOTOR_Phase(4);//
							LED_B=~LED_B;
					}
					if(MOTOR_Get_G()==G_IN[5])//霍尔状态6
					{
							MOTOR_Phase(1);//
							LED_B=~LED_B;
					}	
				MOTOR_ON_Satus=MOTOR_ON;					
			 }
		 else 
				 {
				 MOTOR_ON_Satus=MOTOR_OFF;
				 MOTOR_Phase(7);//
				 }
				EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI1_IRQHandler(void)
{
			if(MOTOR_Satus==MOTOR_STANDBY)
			 {
			 			 MOTOR_ON_Satus=MOTOR_OFF;	
			 			 MOTOR_Phase(0);//
			 }
	else if (MOTOR_Satus==MOTOR_CW)
			 {
					if(MOTOR_Get_G()==G_IN[3])//霍尔状态4
					{
							MOTOR_Phase(2);//
							LED_R=~LED_R;
					}
					if(MOTOR_Get_G()==G_IN[0])//霍尔状态1
					{
							MOTOR_Phase(5);//
							LED_R=~LED_R;
					}	
				MOTOR_ON_Satus=MOTOR_ON;	
			 }
	else if (MOTOR_Satus==MOTOR_CCW)
			 {

					if(MOTOR_Get_G()==G_IN[4])//霍尔状态5
					{
							MOTOR_Phase(6);//
							LED_R=~LED_R;
					}
					if(MOTOR_Get_G()==G_IN[1])//霍尔状态2
					{
							MOTOR_Phase(3);//
							LED_R=~LED_R;
					}	
				MOTOR_ON_Satus=MOTOR_ON;	
			 }
		 else 
				 {
				 MOTOR_ON_Satus=MOTOR_OFF;
				 MOTOR_Phase(7);//
				 }
				
		EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE2上的中断标志位  
}

void EXTI0_IRQHandler(void)
{

				if(MOTOR_Satus==MOTOR_STANDBY)
			 {
			 			 MOTOR_ON_Satus=MOTOR_OFF;	
			 			 MOTOR_Phase(0);//
			 }
	else if (MOTOR_Satus==MOTOR_CW)
			 {
					if(MOTOR_Get_G()==G_IN[5])//霍尔状态6
					{
							MOTOR_Phase(4);//
							LED_G=~LED_G;
					}
					if(MOTOR_Get_G()==G_IN[2])//霍尔状态3
					{
							MOTOR_Phase(1);//
							LED_G=~LED_G;
					}			
					MOTOR_ON_Satus=MOTOR_ON;	
			 }
	else if (MOTOR_Satus==MOTOR_CCW)
			 {
						if(MOTOR_Get_G()==G_IN[3])//霍尔状态4
						{
								MOTOR_Phase(5);//
								LED_G=~LED_G;
						}
						if(MOTOR_Get_G()==G_IN[0])//霍尔状态1
						{
								MOTOR_Phase(2);//
								LED_G=~LED_G;
						}		
						MOTOR_ON_Satus=MOTOR_ON;	
			 }
	 else 
			 {
			 MOTOR_ON_Satus=MOTOR_OFF;
			 MOTOR_Phase(7);//
			 }
			
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE3上的中断标志位  
}

u8 RCC_Initial(void)
{
RCC_DeInit();//将外设RCC寄存器重设为缺省值

RCC_HSEConfig(RCC_HSE_ON);//设置外部高速晶振（HSE）

	if(RCC_WaitForHSEStartUp()==SUCCESS)
	{

			
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//设置AHB时钟（HCLK）

		RCC_PCLK1Config(RCC_HCLK_Div2);//设置低速AHB时钟（PCLK1）

		RCC_PCLK2Config(RCC_HCLK_Div1);//设置低速AHB时钟（PCLK2）
			
		FLASH_SetLatency(FLASH_Latency_2);
			
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//设置PLL时钟源及倍频系数
			
		RCC_PLLCmd(ENABLE);//使能或者失能PLL
			
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
			
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//设置系统时钟（SYSCLK）

		while(RCC_GetSYSCLKSource()!=0x08);
		return 1;
	}
return 0;
}



