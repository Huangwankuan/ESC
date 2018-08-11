//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////	 
/* NRF24L01ʹ�ñʼ�
����SPI��
MISO ��������
MOSI �����������
SCK �����������
NSS �������  �����Ӳ�� ���������������Ӳ��SPI���ƣ�  ������������
CE  NRF24L01����/����ʹ��   �������
IRQ �ж������  �����ⲿ�ж�  ��������   �͵�ƽʹ��  �ⲿ�ж�  ʹ��  �½��ز����ж�


����CE��
����״̬   ѡ�����ģʽ��   ����  1 �ߵ�ƽ
����״̬   ѡ����ģʽ��   ����  �������ģʽ  ����д��TX_FIFO ���߱���>10US   ���� �ͳ�һ������


����IRQ����������������Գ����͵�ƽ�жϣ�
		���Ͷ�
		1.����һ������ ��ȥ  �յ�ACK   ���� ��ʾ���ͳɹ�
		2.�ط����ݰ��ﵽ������   

		���ն�
		1.�ɹ����յ�����
		

����ƽ�����صļ��
	���ͳɹ�    ���Ͷ˵�CE�ź�IRQ��֮��  <1MS  �ļ��
	����ʧ�� �ﵽ�ط������������ն�û��ACK��  >3MS  �ļ��
*/
//////////////////////////////////////////////////////////////////////////////////	 
//Editor��Huangwankuan
//VERSION:1.0
//2017.2.26


/*
����ʹ�÷���:
���Ͷˣ�
   SPI��ʼ��  ����SPI.C     ����Ҫ����������
	 24L01��ʼ��  ����void NRF24L01_Initial(void)   ����Ҫ����������
	 TXģʽ����  void NRF24L01_TX_Mode(void);					//����Ϊ����ģʽ  �����õ���������
	 ����һ������  u8 NRF24L01_TxPacket(u8 *txbuf);				//����һ����������   �����õ���������
	 
���նˣ�
      SPI��ʼ��  ����SPI.C     ����Ҫ����������
			24L01��ʼ��  ����void NRF24L01_Initial(void)   ����Ҫ����������
			�ж�ģʽ���������ⲿ�жϳ�ʼ��  ����EXTI.c 
			��ѯģʽ������������Ҫ�����жϳ�ʼ��
			RXģʽ����  void NRF24L01_RX_Mode(void);					//����Ϊ����ģʽ  �����õ���������
			�ж�ģʽ��������
		void EXTI0_IRQHandler(void)
{
  	u8 sta;	
	
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־	  
	
	if(sta&RX_OK)//���յ�����
	{	
		NRF24L01_Read_Buf(RD_RX_PLOAD,Data_buf,RX_PLOAD_WIDTH);//��ȡ���� 
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
	}	
	
	EXTI_ClearITPendingBit(EXTI_Line0);//���EXTI��·����λ
}

			��ѯģʽ����������һ��������  u8 NRF24L01_RxPacket(u8 *txbuf);				//����һ����������   �����õ���������
*/
//////////////////////////////////////////////////////////////////////////////////	 



#include "24L01.h"
#include "delay.h"
#include "SPI.h"
//const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x33,0x33,0x33,0x33,0x33}; //���͵�ַ
//const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x33,0x33,0x33,0x33,0x33};//���յ�ַ
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02}; //���յ�ַ

//��ʼ��24L01��IO��
//
void NRF24L01_Initial(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��  	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��ָ��IO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO	
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PB0 ���� ����  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_4);//����				
  GPIO_SetBits(GPIOB,GPIO_Pin_11);//����		

  SPI1_Initial();
	
	NRF24L01_CE=0; 			//ʹ��24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}




//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XCC,0XCC,0XCC,0XCC,0XCC};
	u8 i;	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0XCC)
		{
			return 1;//���24L01����	
		}
	}		
	return 0;		 //��⵽24L01
}	 	 


//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI1_Read_Write_Byte(reg);//���ͼĴ����� 
  	SPI1_Read_Write_Byte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}


//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����		
  	SPI1_Read_Write_Byte(reg);   //���ͼĴ�����
  	reg_val=SPI1_Read_Write_Byte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	


//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //ʹ��SPI����
  	status=SPI1_Read_Write_Byte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_Read_Write_Byte(0XFF);//��������
  	NRF24L01_CSN=1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}




//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI1_Read_Write_Byte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_Read_Write_Byte(*pBuf++); //д������	 *pbuf++ ��ȡBUF��ֵ �ٽ�BUF�ĵ�ַ++
  	NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}		








//����NRF24L01����һ������
//txbuf:�����������׵�ַ  �ɶ���ָ������ƶ�
//����ֵ:0��������ɣ�����1û�н���
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;	
	
	while(NRF24L01_IRQ!=0);//�ȴ��������	
	
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //�������/����/����ط���־λ
	//������������DS�����־λ�ſɽ���ͨѶ
	if(sta&RX_OK)//���յ�����      
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��FIFO�Ĵ����ж�ȡ����  �����ֽڳ���32�ֽ�
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ���    ��ȡ��Ϻ����  �Խ����´�FIFO�Ĵ�������
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}				




//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
	  NRF24L01_CE=0;	  
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//ָ��ָ��RX_ADDRESS дRX�ڵ��ַ  ����ֽ� 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}						 







//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
		u8 sta;	
	  u8 feedback=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�  ׼�����뷢��ģʽ
		NRF24L01_CE=1;//������������	
		delay_us(20); //������ʱ >10us
		NRF24L01_CE=0;//��������
	
	while((NRF24L01_IRQ!=0)&&(feedback<100))//���͹���һֱΪ1  �͵�ƽ����ʾ�յ�ACK��ﵽ����ط�����
		{
			feedback++;
			delay_us(100);
		}
	//while((NRF24L01_IRQ!=0));//���͹���һֱΪ1  �͵�ƽ����ʾ�յ�ACK��ﵽ����ط�����
	
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); ////�������/����/����ط���־λ
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ���    ��ȡ��Ϻ����  �Խ����´�FIFO�Ĵ�������
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		 return TX_OK;
	}
		return sta;//����ԭ����ʧ��
}




//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
		NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//ָ��ָ��TX_ADDRESS дRX�ڵ��ַ  ����ֽ� 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x05);//�����Զ��ط����ʱ��:250us + 86us;����Զ��ط�����:5��
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ƶ��+40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
}

