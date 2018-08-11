#ifndef __PID_H
#define __PID_H

//好盈无刷电机2150KV
#define SPEED_UP 380//
#define SPEED_DOWN 30//

typedef struct {

int  SPEED_Now;//

int  Error_Pre;//
int  Error_Last;//
int  Error_Now;//

//float Ti;//微分时间常数
//float Td;//积分时间常数
//float T;//采样周期
float KP;//比例常数  
float KI;//积分常数  KI=KP*(T/Ti)
float KD;//微分常数  KD=KP*(Td/T)

int  PID_OUT;//PID速度输出值
int  PID_MAX;//PID速度输出值
int  PID_MIN;//PID速度输出值
int  PID_SET;//PID速度设置值

int  PID_T_UP;//速度上限
int  PID_T_DOWN;//速度下限


}PID_Value;

void PID_Handle(PID_Value *PID);
void PID_Initial(float KP,float KI,float KD,PID_Value *PID);
#endif

