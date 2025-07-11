#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
/*
引脚：
	内圈PWM：
		TIM1通道1：PA8
		TIM1通道2：PE11
		TIM1通道3：PE13
		TIM1通道4：PA11
	外圈PWM：
		TIM8通道1：PC6
		TIM8通道2：PC7
		TIM8通道3：PC8
		TIM8通道4：PC9
*/

#define MOTOR_MAX_PWM (8400)

#define Motor_A_IN1 PAout(8) //端口映射
#define Motor_A_IN2 PAout(11) //端口映射
#define Motor_B_IN1 PEout(11) //端口映射
#define Motor_B_IN2 PEout(13) //端口映射

#define Motor_C_IN1 PCout(6) //端口映射
#define Motor_C_IN2 PCout(7) //端口映射
#define Motor_D_IN1 PCout(8) //端口映射
#define Motor_D_IN2 PCout(9) //端口映射

typedef struct{
	int16_t g_encoder;
	int16_t pwm;
	int16_t coder_v;
	int16_t target_coder;
	int16_t PWM_MAX;
}motor;

extern motor motor_A, motor_B, motor_C, motor_D;

void Motor_Init(void);//统一初始化
void Timer1_PWM_Init(u16 arr,u16 psc);//TIM1 PWM初始化
void Timer8_PWM_Init(u16 arr, u16 psc);//TIM1 PWM初始化
void PWM_Load(motor* moto, int32_t pwmin);

#endif

