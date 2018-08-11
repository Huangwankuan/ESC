#include "CAN.h"

u8 CAN_Read_DATA[8];
void CAN1_Initial(void){
	
CAN_InitTypeDef CAN_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef 	NVIC_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܻ���ʧ��AFIO


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx	�Ĵ���
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx	�Ĵ���

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


CAN_FilterInitStructure.CAN_FilterNumber = 0;//������0-13
CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
CAN_FilterInitStructure.CAN_FilterIdHigh = 0;//ID
CAN_FilterInitStructure.CAN_FilterIdLow = 0;
CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;//MASK
CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;//FIFO-0 FIFO-1
CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������
CAN_FilterInit(&CAN_FilterInitStructure);//

CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.	
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


//CAN �������ݺ���
//Data_Send_Buf����ָ��   Lenָ��ָ������ĳ���
//����0 ���ͳɹ�
//����1 ����ʧ��
u8 CAN_SEND(u8 *Data_Send_Buf,u8 Len)
{
CanTxMsg TxMessage;
u8 num_mailbox;
int i=0;
	
TxMessage.StdId = 0x3FF;//��׼��ʶ��
TxMessage.ExtId = 0xFF;//��չ��ʶ��
TxMessage.IDE = CAN_ID_STD;//ʹ�ñ�׼֡����
TxMessage.RTR = CAN_RTR_DATA;//����֡
TxMessage.DLC = Len;//Ҫ�������ݵĳ���

	for(i=0;i<Len;i++)
	{
		TxMessage.Data[i]=Data_Send_Buf[i];
	}
		
num_mailbox=CAN_Transmit(CAN1,&TxMessage);//��Ҫ���͵�����д��Ĵ���
	i=0;
while((CAN_TransmitStatus(CAN1,num_mailbox)==CAN_TxStatus_Failed)&&i<0XFFF)
	{
		i++;
		if(i>=0xFF0)
		{return 1;}
	}

	return 0;

}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;					 // ��׼��ʶ�� 
  TxMessage.ExtId=0x12;				   // ������չ��ʾ�� 
  TxMessage.IDE=CAN_Id_Standard; // ��׼֡
  TxMessage.RTR=CAN_RTR_Data;		 // ����֡
  TxMessage.DLC=len;						// Ҫ���͵����ݳ���
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}

