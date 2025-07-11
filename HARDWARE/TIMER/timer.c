#include "timer.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "motor.h"
#include "pid.h"
#include "adc.h"

//int ms_flag = 0;//��ʱ�����أ�1Ϊ������0Ϊ�ر�
//uint32_t system_millis = 0; // ϵͳ���������
uint8_t Pid_calculate_Flag = 1;//PID���ر�־λ��Ĭ�Ͽ���

void TIM7_Int_Init(u16 arr,u16 psc)//������ʱ��
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//ʹ��TIM7ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����

	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);//����ʱ��7�����ж�
	TIM_Cmd(TIM7, ENABLE);//ʹ�ܶ�ʱ��7

	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;//��ʱ��7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//����pid 
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
	{
//		//��ʱ��
//		if(ms_flag==1) system_millis++;
//		else system_millis=0;
		
		//pidÿ10ms��һ��
		 if(Pid_calculate_Flag==1)
		 {
				X_PID_OUT = PID_Calculate(&X_PID,X_Set,ADC_Values[0]);	//X��
				Y_PID_OUT = PID_Calculate(&Y_PID,Y_Set,ADC_Values[1]);	//Y��
//				Z_PID_OUT = PID_Calculate(&Z_PID,Z_Set,ADC_Values[2]);	//Z��
		 }
				PWM_Load(&motor_A,X_PID_OUT);//װ�ص��
				PWM_Load(&motor_B,Y_PID_OUT);//װ�ص��
				// PWM_Load(&motor_C,Z_PID_OUT);//װ�ص��
				// PWM_Load(&motor_D,Z_PID_OUT);//װ�ص��

		}
    TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //����жϱ�־λ(��0��ʼ����)
}

