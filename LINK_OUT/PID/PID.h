#ifndef __PID_H
#define __PID_H

//��ӯ��ˢ���2150KV
#define SPEED_UP 380//
#define SPEED_DOWN 30//

typedef struct {

int  SPEED_Now;//

int  Error_Pre;//
int  Error_Last;//
int  Error_Now;//

//float Ti;//΢��ʱ�䳣��
//float Td;//����ʱ�䳣��
//float T;//��������
float KP;//��������  
float KI;//���ֳ���  KI=KP*(T/Ti)
float KD;//΢�ֳ���  KD=KP*(Td/T)

int  PID_OUT;//PID�ٶ����ֵ
int  PID_MAX;//PID�ٶ����ֵ
int  PID_MIN;//PID�ٶ����ֵ
int  PID_SET;//PID�ٶ�����ֵ

int  PID_T_UP;//�ٶ�����
int  PID_T_DOWN;//�ٶ�����


}PID_Value;

void PID_Handle(PID_Value *PID);
void PID_Initial(float KP,float KI,float KD,PID_Value *PID);
#endif

