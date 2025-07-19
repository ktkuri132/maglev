#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"

extern uint8_t Pid_calculate_Flag;//ȫ��PID��־λ

extern int ms_flag;//��ʱ�����أ�1Ϊ������0Ϊ�ر�
extern uint32_t system_millis; // ϵͳ���������

extern uint16_t ADC_Low_Values[3];//

void TIM7_Int_Init(u16 arr,u16 psc);//������ʱ��
void TIM7_IRQHandler(void);

#endif


