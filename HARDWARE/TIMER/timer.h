#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"

extern uint8_t Pid_calculate_Flag;//全部PID标志位

extern int ms_flag;//计时器开关，1为开启，0为关闭
extern uint32_t system_millis; // 系统毫秒计数器

extern uint16_t ADC_Low_Values[3];//

void TIM7_Int_Init(u16 arr,u16 psc);//基本定时器
void TIM7_IRQHandler(void);

#endif


