#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "led.h"
#include "oled.h"
#include "adc.h"
#include "motor.h"
#include "pid.h"
#include "timer.h"
#include "tof.h"
#include "usart2.h"	
#include "jy61p.h"	
#include "usart3.h"	
#include "control.h"
#include "kalman.h"


int main(void)
{
	/*===========================��ʼ��===========================*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);//��ʼ����ʱ����
	uart_init(115200);//����ͨѶ��ʼ��
	OLED_Init();//OLED��ʼ��
    Adc_Init();//ADC��ʼ��
	DMA_Config();//DMA��ʼ��
	Motor_Init();//PWM��ʼ��
	PID_Init();//PID��ʼ��
	TIM7_Int_Init(1000-1,84-1);//��ʱ����ʼ��1ms
	uart2_init(9600);//����2��ʼ��(������ʹ��)������ڼ���ǰ���ʼ��
	TOF10120_Init();
	usart3_init(115200);//����3��ʼ����JY61Pʹ�ã�
    Kalman_Init(&kfp_x);
    Kalman_Init(&kfp_y);
    Kalman_Init(&kfp_z);
    PWM_Load(&motor_C,-6000);
    PWM_Load(&motor_D,-6000);

	while(1)
	{
		
	  OLED_ShowString(0,0,"X:",8,1);
	  OLED_ShowFNum(20,0,ADC_Values[0],6,8,1);
	  OLED_ShowString(0,8,"Y:",8,1);
	  OLED_ShowFNum(20,8,ADC_Values[1],6,8,1);
	  OLED_ShowString(0,16,"Z:",8,1);
	  OLED_ShowFNum(20,16,ADC_Values[2],6,8,1);
      OLED_ShowString(0,8,"dianliu1:",8,1);
	  OLED_ShowFNum(20,8,ADC_Values[3],6,8,1);
	  OLED_ShowString(0,16,"dianliu2:",8,1);
	  OLED_ShowFNum(20,16,ADC_Values[4],6,8,1);
		
	  OLED_ShowString(0,24,"len:",8,1);
	  OLED_ShowNum(40,24,length_val,3,8,1);
	  OLED_ShowString(70,24,"mm",8,1);
		
//	  OLED_ShowString(0,32,"Roll:",8,1);
//	  OLED_ShowFNum(40,32,SAngle.Angle[0],6,8,1);
//	  OLED_ShowString(0,40,"Pitch:",8,1);
//	  OLED_ShowFNum(40,40,SAngle.Angle[1],6,8,1);
//	  OLED_ShowString(0,48,"Yaw:",8,1);
//	  OLED_ShowFNum(40,48,SAngle.Angle[2],6,8,1);
		
	  OLED_Refresh();
		
//		TIM_SetCompare1(TIM1,2100);//����ͨ��һռ�ձ�
//		TIM_SetCompare2(TIM1,4200);//����ͨ��һռ�ձ�
//		TIM_SetCompare3(TIM1,6300);//����ͨ����ռ�ձ�
//		TIM_SetCompare4(TIM1,8000);//����ͨ����ռ�ձ�
//		
//		TIM_SetCompare1(TIM8,2100);//����ͨ��һռ�ձ�
//      TIM_SetCompare2(TIM8,4200);//����ͨ����ռ�ձ�
//      TIM_SetCompare3(TIM8,6300);//����ͨ����ռ�ձ�
//      TIM_SetCompare4(TIM8,8000);//����ͨ����ռ�ձ�
//		PWM_Load(&motor_A,-1000);
//		PWM_Load(&motor_B,-1000);
        printf("%d,%d,%f,%f,%d,%d,%d,%d\r\n",
            ADC_Values[0], ADC_Values[1],
            kfp_x.out, kfp_y.out,
            ADC_Low_Values[0], ADC_Low_Values[1],
            X_PID_OUT, Y_PID_OUT
       );
       PID_Update();
		
	   TOF10120_GetData();//���
	}		

}

