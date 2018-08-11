#ifndef __CAN_H
#define __CAN_H
#include "stm32f10x.h"

void CAN1_Initial(u8 num);
u8 CAN_SEND(u16 Std_ID,u8 *Data_Send_Buf,u8 Len);
//u8 Can_Send_Msg(u8* msg,u8 len);


#endif
