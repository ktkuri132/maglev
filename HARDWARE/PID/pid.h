#ifndef __PID_H
#define __PID_H
#include <stdint.h>

extern float X_Set;//左轮电机目标值
extern float Y_Set;//右轮电机目标值
extern float Z_Set;//转向目标值

extern int32_t X_PID_OUT;//左轮PWM输出值
extern int32_t Y_PID_OUT;//右轮PWM输出值
extern int32_t Z_PID_OUT;//转向输出值


// PID结构体定义
typedef struct
{   float KP;//比例
    float KI;//微分
	  float KD;//积分
	  float Target;//目标值
    float Err;//当前误差值
    float LastErr;//上一时刻的误差值
	  float PenultErr;//上上一时刻的误差值
    float Integral;//积分累加值
	  float limit;//限幅
    float a;
        float b;
        float c;
	  int32_t PID_Out;
}PID_TypeDef;

extern PID_TypeDef X_PID, Y_PID,Z_PID;

void PID_Init(void);//PID初始化
int16_t PID_Calculate(PID_TypeDef *PID,float TargetValue,int32_t CurrentValue);//PID计算
void X_PID_SET(float KP,float KI,float KD);//左轮电机PID设置
void Y_PID_SET(float KP,float KI,float KD);//右轮电机PID设置
void Z_PID_SET(float KP,float KI,float KD);//转向PID设置
void PID_Update();


#endif

