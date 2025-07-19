#include "pid.h"
#include "usart.h"
#include <stdint.h>

float X_Set=2924;//X轴目标值
float Y_Set=2915;//Y轴目标值
float Z_Set=0;//Z轴目标值

int32_t X_PID_OUT;//左轮PWM输出值
int32_t Y_PID_OUT;//右轮PWM输出值
int32_t Z_PID_OUT;//转向输出值

int32_t XI_PID_OUT;//左轮电流输出值
int32_t YI_PID_OUT;//右轮电流输出值

//定义结构体
PID_TypeDef X_PID;//左轮电机PID
PID_TypeDef Y_PID;//右轮电机PID
PID_TypeDef Z_PID;//转向PID

PID_TypeDef XI_PID;//电流PID
PID_TypeDef YI_PID;//电流PID

void PID_Init(void)//PID初始化
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
	  // 1.计算偏差
    PID->Err =TargetValue - CurrentValue;//计算当前误差值=目标值-当前测量值
	  // 2.累计偏差=当前误差+上一次误差
    PID->Integral += PID->Err;
	  // 3.积分限幅
    PID->Integral=PID->Integral>1000?1000:PID->Integral<(-1000)?(-1000):PID->Integral;	
	  // 4.PID算法实现
    PID->PID_Out = PID->KP * PID->Err 								  /*比例*/
				         + PID->KI * PID->Integral              /*积分*/
			           + PID->KD * (PID->Err - PID->LastErr);	/*微分*/
	  // 5.更新上一次误差
	  PID->LastErr = PID->Err;//保留上一次的偏差
	  // 6.限幅
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

void X_PID_SET(float KP,float KI,float KD)//左轮电机PID设置
{
	X_PID.KP=KP;
	X_PID.KI=KI;
	X_PID.KD=KD;
}
	
void Y_PID_SET(float KP,float KI,float KD)//右轮电机PID设置
{
	Y_PID.KP=KP;
	Y_PID.KI=KI;
	Y_PID.KD=KD;
}

void Z_PID_SET(float KP,float KI,float KD)//转向PID设置
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