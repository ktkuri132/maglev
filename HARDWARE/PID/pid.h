#ifndef __PID_H
#define __PID_H
#include <stdint.h>

extern float X_Set;//���ֵ��Ŀ��ֵ
extern float Y_Set;//���ֵ��Ŀ��ֵ
extern float Z_Set;//ת��Ŀ��ֵ

extern int32_t X_PID_OUT;//����PWM���ֵ
extern int32_t Y_PID_OUT;//����PWM���ֵ
extern int32_t Z_PID_OUT;//ת�����ֵ


// PID�ṹ�嶨��
typedef struct
{   float KP;//����
    float KI;//΢��
	  float KD;//����
	  float Target;//Ŀ��ֵ
    float Err;//��ǰ���ֵ
    float LastErr;//��һʱ�̵����ֵ
	  float PenultErr;//����һʱ�̵����ֵ
    float Integral;//�����ۼ�ֵ
	  float limit;//�޷�
    float a;
        float b;
        float c;
	  int32_t PID_Out;
}PID_TypeDef;

extern PID_TypeDef X_PID, Y_PID,Z_PID;

void PID_Init(void);//PID��ʼ��
int16_t PID_Calculate(PID_TypeDef *PID,float TargetValue,int32_t CurrentValue);//PID����
void X_PID_SET(float KP,float KI,float KD);//���ֵ��PID����
void Y_PID_SET(float KP,float KI,float KD);//���ֵ��PID����
void Z_PID_SET(float KP,float KI,float KD);//ת��PID����
void PID_Update();


#endif

