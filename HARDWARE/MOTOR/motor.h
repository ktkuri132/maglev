#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
/*
���ţ�
	��ȦPWM��
		TIM1ͨ��1��PA8
		TIM1ͨ��2��PE11
		TIM1ͨ��3��PE13
		TIM1ͨ��4��PA11
	��ȦPWM��
		TIM8ͨ��1��PC6
		TIM8ͨ��2��PC7
		TIM8ͨ��3��PC8
		TIM8ͨ��4��PC9
*/

#define MOTOR_MAX_PWM (8400)

#define Motor_A_IN1 PAout(8) //�˿�ӳ��
#define Motor_A_IN2 PAout(11) //�˿�ӳ��
#define Motor_B_IN1 PEout(11) //�˿�ӳ��
#define Motor_B_IN2 PEout(13) //�˿�ӳ��

#define Motor_C_IN1 PCout(6) //�˿�ӳ��
#define Motor_C_IN2 PCout(7) //�˿�ӳ��
#define Motor_D_IN1 PCout(8) //�˿�ӳ��
#define Motor_D_IN2 PCout(9) //�˿�ӳ��

typedef struct{
	int16_t g_encoder;
	int16_t pwm;
	int16_t coder_v;
	int16_t target_coder;
	int16_t PWM_MAX;
}motor;

extern motor motor_A, motor_B, motor_C, motor_D;

void Motor_Init(void);//ͳһ��ʼ��
void Timer1_PWM_Init(u16 arr,u16 psc);//TIM1 PWM��ʼ��
void Timer8_PWM_Init(u16 arr, u16 psc);//TIM1 PWM��ʼ��
void PWM_Load(motor* moto, int32_t pwmin);

#endif

