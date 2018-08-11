#include "CAN.h"

u8 CAN_Read_DATA[8];
void CAN1_Initial(void){
	
CAN_InitTypeDef CAN_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef 	NVIC_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);//使能或者失能APB1外设时钟
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能或者失能APB2外设时钟
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能或者失能AFIO


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx	寄存器

CAN_InitStructure.CAN_TTCM = DISABLE;
CAN_InitStructure.CAN_ABOM = DISABLE;
CAN_InitStructure.CAN_AWUM = DISABLE;
CAN_InitStructure.CAN_NART = ENABLE;
CAN_InitStructure.CAN_RFLM = DISABLE;
CAN_InitStructure.CAN_TXFP = DISABLE;
CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
CAN_InitStructure.CAN_Prescaler = 4;
CAN_Init(CAN1,&CAN_InitStructure);//


CAN_FilterInitStructure.CAN_FilterNumber = 0;//过滤器0-13
CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
CAN_FilterInitStructure.CAN_FilterIdHigh = 0;//ID
CAN_FilterInitStructure.CAN_FilterIdLow = 0;
CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;//MASK
CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;//FIFO-0 FIFO-1
CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器
CAN_FilterInit(&CAN_FilterInitStructure);//

CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.	
CAN_ITConfig(CAN1,CAN_IT_TME ,ENABLE);//?????????CAN??

NVIC_InitStructure.NVIC_IRQChannel=USB_HP_CAN1_TX_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//
NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

}


//CAN 发送数据函数
//Data_Send_Buf数组指针   Len指针指向数组的长度
//返回0 发送成功
//返回1 发送失败
u8 CAN_SEND(u8 *Data_Send_Buf,u8 Len)
{
CanTxMsg TxMessage;
u8 num_mailbox;
int i=0;
	
TxMessage.StdId = 0x3FF;//标准标识符
TxMessage.ExtId = 0xFF;//扩展标识符
TxMessage.IDE = CAN_ID_STD;//使用标准帧发送
TxMessage.RTR = CAN_RTR_DATA;//数据帧
TxMessage.DLC = Len;//要发送数据的长度

	for(i=0;i<Len;i++)
	{
		TxMessage.Data[i]=Data_Send_Buf[i];
	}
		
num_mailbox=CAN_Transmit(CAN1,&TxMessage);//将要发送的数据写入寄存器
	i=0;
while((CAN_TransmitStatus(CAN1,num_mailbox)==CAN_TxStatus_Failed)&&i<0XFFF)
	{
		i++;
		if(i>=0xFF0)
		{return 1;}
	}

	return 0;

}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;					 // 标准标识符 
  TxMessage.ExtId=0x12;				   // 设置扩展标示符 
  TxMessage.IDE=CAN_Id_Standard; // 标准帧
  TxMessage.RTR=CAN_RTR_Data;		 // 数据帧
  TxMessage.DLC=len;						// 要发送的数据长度
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}

