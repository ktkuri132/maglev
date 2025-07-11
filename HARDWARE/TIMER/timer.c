#include "timer.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "motor.h"
#include "pid.h"
#include "adc.h"

//int ms_flag = 0;//计时器开关，1为开启，0为关闭
//uint32_t system_millis = 0; // 系统毫秒计数器
uint8_t Pid_calculate_Flag = 1;//PID开关标志位（默认开）

void TIM7_Int_Init(u16 arr,u16 psc)//基本定时器
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//使能TIM7时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子

	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);//允许定时器7更新中断
	TIM_Cmd(TIM7, ENABLE);//使能定时器7

	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;//定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//计算pid 
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
//		//计时器
//		if(ms_flag==1) system_millis++;
//		else system_millis=0;
		
		//pid每10ms读一次
		 if(Pid_calculate_Flag==1)
		 {
				X_PID_OUT = PID_Calculate(&X_PID,X_Set,ADC_Values[0]);	//X轴
				Y_PID_OUT = PID_Calculate(&Y_PID,Y_Set,ADC_Values[1]);	//Y轴
//				Z_PID_OUT = PID_Calculate(&Z_PID,Z_Set,ADC_Values[2]);	//Z轴
		 }
				PWM_Load(&motor_A,X_PID_OUT);//装载电机
				PWM_Load(&motor_B,Y_PID_OUT);//装载电机
				// PWM_Load(&motor_C,Z_PID_OUT);//装载电机
				// PWM_Load(&motor_D,Z_PID_OUT);//装载电机

		}
    TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位(从0开始计数)
}

