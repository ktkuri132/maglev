#include "motor.h"
#include "usart.h"
#include "timer.h" 
#include "pid.h"

motor motor_A = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_B = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_C = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_D = {0,0,0,0,MOTOR_MAX_PWM};

void Motor_Init(void)//ͳһ��ʼ��
{
	Timer1_PWM_Init(8400-1,50-1);//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.
	Timer8_PWM_Init(8400-1,50-1);//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.
}

//TIM1 PWM���ֳ�ʼ�� ��PA9,PA10Ϊusart1���ţ�
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
// ͨ�����ã�
//  |- CH1->PA8 -->X-->�ڻ�AIN1
//  |- CH2->PE11-->Y-->�ڻ�BIN1
//  |- CH3->PE13-->Y-->�ڻ�BIN2
//  |- CH4->PA11-->X-->�ڻ�AIN2
void Timer1_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);//ʹ��PORTAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//ʹ��PORTEʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);//GPIOA8����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOE11����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//GPIOE13����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOA11����Ϊ��ʱ��1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;//TIM1_CH1-->PA8,TIM1_CH4-->PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA8 PA11 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;//TIM1_CH2-->PE11,TIM1_CH3-->PE13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��PA8 PA11 
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);//��ʼ����ʱ��1
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//ѡ��ʱ��ģʽ(��ʼ������Ƚ�,TIMx_CNT<TIM_CCRx��ʱ��Ϊ��Ч��ƽ)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//�������:TIM����Ƚϼ��Ը�(��Ч��ƽΪ��)
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);//��ʼ��TIM1������Ƚ�ͨ��1
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);//��ʼ��TIM1������Ƚ�ͨ��2
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);//��ʼ��TIM1������Ƚ�ͨ��3
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);//��ʼ��TIM1������Ƚ�ͨ��4
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC2Ԥװ�ؼĴ���ʹ��
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC3Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC4Ԥװ�ؼĴ���ʹ��

	TIM_SetCompare1(TIM1,0);//����ͨ��һռ�ձ�
	TIM_SetCompare2(TIM1,0);//����ͨ����ռ�ձ�
	TIM_SetCompare3(TIM1,0);//����ͨ����ռ�ձ�
	TIM_SetCompare4(TIM1,0);//����ͨ����ռ�ձ�
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);// ʹ��PWM������߼���ʱ�����У�
  TIM_ARRPreloadConfig(TIM1, ENABLE);// ʹ��TIM1���Զ��ؼ��ؼĴ�����ARPEʹ�ܣ�
	TIM_Cmd(TIM1,ENABLE);//ʹ��TIM1
}

//TIM8 PWM���ֳ�ʼ�� ��PA9,PA10Ϊusart1���ţ�
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
// ͨ�����ã�
//  |- CH1->PC6-->X-->�⻷AIN1
//  |- CH2->PC7-->Y-->�⻷BIN1
//  |- CH3->PC8-->Y-->�⻷BIN2
//  |- CH4->PC9-->X-->�⻷AIN2
void Timer8_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//TIM8ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);//ʹ��PORTCʱ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);//GPIOC6����Ϊ��ʱ��8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);//GPIOC7����Ϊ��ʱ��8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);//GPIOC8����Ϊ��ʱ��8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);//GPIOC9����Ϊ��ʱ��8

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//TIM8_CH1-->PC6,TIM8_CH2-->PC7,TIM8_CH3-->PC8,TIM8_CH4-->PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��PC6 PC7 PC8 PC9  
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);//��ʼ����ʱ��8
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//ѡ��ʱ��ģʽ(��ʼ������Ƚ�,TIMx_CNT<TIM_CCRx��ʱ��Ϊ��Ч��ƽ)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//�������:TIM����Ƚϼ��Ը�(��Ч��ƽΪ��)
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OC1Init(TIM8,&TIM_OCInitStruct);//��ʼ��TIM8������Ƚ�ͨ��1
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);//��ʼ��TIM8������Ƚ�ͨ��2
	TIM_OC3Init(TIM8,&TIM_OCInitStruct);//��ʼ��TIM8������Ƚ�ͨ��3
	TIM_OC4Init(TIM8,&TIM_OCInitStruct);//��ʼ��TIM8������Ƚ�ͨ��4
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC2Ԥװ�ؼĴ���ʹ��
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC3Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC4Ԥװ�ؼĴ���ʹ��

	TIM_SetCompare1(TIM8,0);//����ͨ��һռ�ձ�
	TIM_SetCompare2(TIM8,0);//����ͨ����ռ�ձ�
	TIM_SetCompare3(TIM8,0);//����ͨ����ռ�ձ�
	TIM_SetCompare4(TIM8,0);//����ͨ����ռ�ձ�
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);// ʹ��PWM������߼���ʱ�����У�
  TIM_ARRPreloadConfig(TIM8, ENABLE);// ʹ��TIM8���Զ��ؼ��ؼĴ�����ARPEʹ�ܣ�
	TIM_Cmd(TIM8,ENABLE);//ʹ��TIM8
}

/*��ֵ����*/
/*��ڲ�����PID������ɺ������PWMֵ*/
void PWM_Load(motor* moto, int32_t pwmin)
{
	//�޷�
	if(pwmin > moto->PWM_MAX){
		pwmin = moto->PWM_MAX;
	  } 
	else if(pwmin < -moto->PWM_MAX){
		pwmin = -moto->PWM_MAX;
	  }
	moto->pwm = pwmin;	
//  printf("          %d\r\n",pwmin);

  //�����Ŷ�Ӧ����ת,װ��PWMֵ
	if(moto == &motor_A){
		if(moto->pwm > 0){
			TIM_SetCompare1(TIM1,8400);
			TIM_SetCompare4(TIM1,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // ���������
			TIM_SetCompare4(TIM1,8400);
			TIM_SetCompare1(TIM1,(8400+moto->pwm));
		}
	}
	if(moto == &motor_B){
		if(moto->pwm > 0){
			TIM_SetCompare3(TIM1,8400);
			TIM_SetCompare2(TIM1,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // ���������
			TIM_SetCompare2(TIM1,8400);
			TIM_SetCompare3(TIM1,(8400+moto->pwm));
		}
	}
	if(moto == &motor_C){
		if(moto->pwm > 0){
			TIM_SetCompare1(TIM8,8400);
			TIM_SetCompare2(TIM8,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // ���������
			TIM_SetCompare2(TIM8,8400);
			TIM_SetCompare1(TIM8,(8400+moto->pwm));
		}
	}
	if(moto == &motor_D){
		if(moto->pwm > 0){
			TIM_SetCompare4(TIM8,8400);
			TIM_SetCompare3(TIM8,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // ���������
			TIM_SetCompare3(TIM8,8400);
			TIM_SetCompare4(TIM8,(8400+moto->pwm));
		}
	}
}
