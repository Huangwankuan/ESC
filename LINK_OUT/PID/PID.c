#include "PID.h"

void PID_Handle(PID_Value *PID)
{

PID->Error_Now=PID->PID_SET-PID->SPEED_Now;

if(PID->Error_Now>=50)
{PID->PID_OUT=PID->PID_OUT+PID->PID_MAX;}
else if(PID->Error_Now<=-50)
{PID->PID_OUT=PID->PID_OUT-PID->PID_MIN;}
else{
PID->PID_OUT=PID->PID_OUT
+PID->KP*(PID->Error_Now-PID->Error_Last)
+PID->KI*PID->Error_Now
+PID->KD*(PID->SPEED_Now-2*PID->Error_Last+PID->Error_Pre);
}
if(PID->PID_OUT>=620)
{
PID->PID_OUT=620;
}
else if(PID->PID_OUT<=30)
{
PID->PID_OUT=30;
}
else{}
PID->Error_Pre=PID->Error_Last;
PID->Error_Last=PID->Error_Now;
}


void PID_Initial(float KP,float KI,float KD,PID_Value *PID)
{
PID->KP=KP;
PID->KI=KI;
PID->KD=KD;
PID->Error_Pre=0;
PID->Error_Last=0;
PID->PID_MAX=30;
PID->PID_MIN=30;
PID->PID_OUT=0;
}

