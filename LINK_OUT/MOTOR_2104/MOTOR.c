#include "MOTOR.h"

u8 MOTOR_Satus_Last=MOTOR_CW;
u8 MOTOR_Satus =MOTOR_CW;

u8 MOTOR_ON_Satus = MOTOR_OFF;

const u8 G_IN[6]={6,4,5,1,3,2};

void MOTOR_HALL_Initial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
}
	
void MOTOR_GPIO_Initial(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能或者失能APB2外设时钟
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
	
	
}

void MOTOR_Phase(u8 num)
{
	switch(num)// （+ IN高 EN低）  （中--IN低  EN  低）  (低--IN低 EN 高)  
	{
			case 0:IN_A=0;EN_A=0;IN_B=0;EN_B=0;IN_C=0;EN_C=0;break;//均处于待机状态  PWM=1;
		
			case 1:IN_A=1;EN_A=0;IN_B=0;EN_B=0;IN_C=0;EN_C=1;break;//A+ C-
		
			case 2:IN_A=0;EN_A=0;IN_B=1;EN_B=0;IN_C=0;EN_C=1;break;//B+ C-
		
			case 3:IN_A=0;EN_A=1;IN_B=1;EN_B=0;IN_C=0;EN_C=0;break;//A- B+
		
			case 4:IN_A=0;EN_A=1;IN_B=0;EN_B=0;IN_C=1;EN_C=0;break;//A- C+
		
			case 5:IN_A=0;EN_A=0;IN_B=0;EN_B=1;IN_C=1;EN_C=0;break;//B- C+
		
			case 6:IN_A=1;EN_A=0;IN_B=0;EN_B=1;IN_C=0;EN_C=0;break;//A+ B-
		
			case 7:IN_A=0;EN_A=1;IN_B=0;EN_B=1;IN_C=0;EN_C=1;break;//接地制动
	}
}
/*
void MOTOR_Phase(u8 num)
{
	switch(num)// （+ IN高 EN低）  （中--IN低  EN  低）  (低--IN低 EN 高)  
	{
			case 0:IN_A=0;EN_A=0;IN_B=0;EN_B=0;IN_C=0;EN_C=0;break;//均处于待机状态  PWM=1;
		
			case 1:IN_A=1;EN_A=0;IN_B=0;EN_B=0;IN_C=0;EN_C=1;break;//A+ C-
		
			case 2:IN_A=0;EN_A=0;IN_B=1;EN_B=0;IN_C=0;EN_C=1;break;//B+ C-
		
			case 3:IN_A=0;EN_A=1;IN_B=1;EN_B=0;IN_C=0;EN_C=0;break;//A- B+
		
			case 4:IN_A=0;EN_A=1;IN_B=0;EN_B=0;IN_C=1;EN_C=0;break;//A- C+
		
				case 5:IN_A=0;EN_A=0;IN_B=0;EN_B=1;IN_C=1;EN_C=0;break;//B- C+
		
			case 6:IN_A=1;EN_A=0;IN_B=0;EN_B=1;IN_C=0;EN_C=0;break;//A+ B-
		
			case 7:IN_A=0;EN_A=1;IN_B=0;EN_B=1;IN_C=0;EN_C=1;break;//接地制动
	}
}*/
int MOTOR_Get_G(void)
{
    int G;
		G=G1<<2;
		G=G|(G2<<1);
		G=G|(G3);
		return G;
}
void MOTOR_UP(int Status)
{
   if(Status==MOTOR_CW)
	 {
	    switch(MOTOR_Get_G())
			{
			case 6:  MOTOR_Phase(5);break;
						case 4: MOTOR_Phase(6); break;
									case 5: MOTOR_Phase(1); break;
												case 1:  MOTOR_Phase(2);break;
															case 3:  MOTOR_Phase(3);break;
																		case 2:  MOTOR_Phase(4);break;
			}
	 }
	 else if(Status==MOTOR_CCW)
	 {
	    	    switch(MOTOR_Get_G())
			{
			case 6:  MOTOR_Phase(2);break;
						case 4: MOTOR_Phase(3); break;
									case 5: MOTOR_Phase(4); break;
												case 1:  MOTOR_Phase(5);break;
															case 3:  MOTOR_Phase(6);break;
																		case 2:  MOTOR_Phase(1);break;
			}
	 }
	 else
	 {
	 
	 }
}


	/*		case 0:IN_A=0;EN_A=0;IN_B=0;EN_B=0;IN_C=0;EN_C=0;break;//均处于待机状态
			case 1:IN_A=1;EN_A=1;IN_B=0;EN_B=0;IN_C=0;EN_C=1;break;//A+ C-
			case 2:IN_A=0;EN_A=0;IN_B=1;EN_B=1;IN_C=0;EN_C=1;break;//B+ C-
			case 3:IN_A=0;EN_A=1;IN_B=1;EN_B=1;IN_C=0;EN_C=0;break;//A- B+
			case 4:IN_A=0;EN_A=1;IN_B=0;EN_B=0;IN_C=1;EN_C=1;break;//A- C+
			case 5:IN_A=0;EN_A=0;IN_B=0;EN_B=1;IN_C=1;EN_C=1;break;//B- C+
			case 6:IN_A=1;EN_A=1;IN_B=0;EN_B=1;IN_C=0;EN_C=0;break;//A+ B-
			case 7:IN_A=0;EN_A=1;IN_B=0;EN_B=1;IN_C=0;EN_C=1;break;//接地制动*/

