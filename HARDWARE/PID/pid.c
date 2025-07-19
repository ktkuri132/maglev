#include "pid.h"
#include "usart.h"
#include <stdint.h>

float X_Set=2924;//X��Ŀ��ֵ
float Y_Set=2915;//Y��Ŀ��ֵ
float Z_Set=0;//Z��Ŀ��ֵ

int32_t X_PID_OUT;//����PWM���ֵ
int32_t Y_PID_OUT;//����PWM���ֵ
int32_t Z_PID_OUT;//ת�����ֵ

int32_t XI_PID_OUT;//���ֵ������ֵ
int32_t YI_PID_OUT;//���ֵ������ֵ

//����ṹ��
PID_TypeDef X_PID;//���ֵ��PID
PID_TypeDef Y_PID;//���ֵ��PID
PID_TypeDef Z_PID;//ת��PID

PID_TypeDef XI_PID;//����PID
PID_TypeDef YI_PID;//����PID

void PID_Init(void)//PID��ʼ��
{	
    X_PID.KP = -30;
    X_PID.KI = 0;
	X_PID.KD = 0;
	Y_PID.KP = X_PID.KP;
    Y_PID.KI = X_PID.KI;
	Y_PID.KD = X_PID.KD;

    XI_PID.KP = -30;
    XI_PID.KI = 0;
	XI_PID.KD = 0;
	YI_PID.KP = XI_PID.KP;
    YI_PID.KI = XI_PID.KI;
	YI_PID.KD = XI_PID.KD;

    X_PID.Err = 0.0f;
    X_PID.LastErr = 0.0f;
	X_PID.PenultErr = 0.0f;
    X_PID.Integral = 0.0f;
	X_PID.limit = 8400;
	X_PID.PID_Out = 0; 
	
    Y_PID.Err = 0.0f;
    Y_PID.LastErr = 0.0f;
	Y_PID.PenultErr = 0.0f;
    Y_PID.Integral = 0.0f;
	Y_PID.limit = 8400;
	Y_PID.PID_Out = 0;
	
	Z_PID.Err = 0.0f;
    Z_PID.LastErr = 0.0f;
	Z_PID.PenultErr = 0.0f;
    Z_PID.Integral = 0.0f;
	Z_PID.limit = 8400;
	Z_PID.PID_Out = 0;

	XI_PID.Err = 0.0f;
    XI_PID.LastErr = 0.0f;
	XI_PID.PenultErr = 0.0f;
    XI_PID.Integral = 0.0f;
	XI_PID.limit = 8400;
	XI_PID.PID_Out = 0; 
	
    YI_PID.Err = 0.0f;
    YI_PID.LastErr = 0.0f;
	YI_PID.PenultErr = 0.0f;
    YI_PID.Integral = 0.0f;
	YI_PID.limit = 8400;
	YI_PID.PID_Out = 0;
}

int16_t PID_Calculate(PID_TypeDef *PID,float TargetValue,int32_t CurrentValue)
{
	  // 1.����ƫ��
    PID->Err =TargetValue - CurrentValue;//���㵱ǰ���ֵ=Ŀ��ֵ-��ǰ����ֵ
	  // 2.�ۼ�ƫ��=��ǰ���+��һ�����
    PID->Integral += PID->Err;
	  // 3.�����޷�
    PID->Integral=PID->Integral>1000?1000:PID->Integral<(-1000)?(-1000):PID->Integral;	
	  // 4.PID�㷨ʵ��
    PID->PID_Out = PID->KP * PID->Err 								  /*����*/
				         + PID->KI * PID->Integral              /*����*/
			           + PID->KD * (PID->Err - PID->LastErr);	/*΢��*/
	  // 5.������һ�����
	  PID->LastErr = PID->Err;//������һ�ε�ƫ��
	  // 6.�޷�
	  if(PID->PID_Out > PID->limit)
	{
		PID->PID_Out =  PID->limit;
	}
	  else if(PID->PID_Out  < -PID->limit)
	{
		PID->PID_Out  =  -PID->limit;
	}
    return PID->PID_Out;
}

void X_PID_SET(float KP,float KI,float KD)//���ֵ��PID����
{
	X_PID.KP=KP;
	X_PID.KI=KI;
	X_PID.KD=KD;
}
	
void Y_PID_SET(float KP,float KI,float KD)//���ֵ��PID����
{
	Y_PID.KP=KP;
	Y_PID.KI=KI;
	Y_PID.KD=KD;
}

void Z_PID_SET(float KP,float KI,float KD)//ת��PID����
{
	Z_PID.KP=KP;
	Z_PID.KI=KI;
	Z_PID.KD=KD;
}

void PID_Update(){
    Y_PID.KP=X_PID.KP;
	Y_PID.KI=X_PID.KI;
	Y_PID.KD=X_PID.KD;    
}