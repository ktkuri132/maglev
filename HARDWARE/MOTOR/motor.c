#include "motor.h"
#include "usart.h"
#include "timer.h" 
#include "pid.h"

motor motor_A = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_B = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_C = {0,0,0,0,MOTOR_MAX_PWM};
motor motor_D = {0,0,0,0,MOTOR_MAX_PWM};

void Motor_Init(void)//统一初始化
{
	Timer1_PWM_Init(8400-1,50-1);//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.
	Timer8_PWM_Init(8400-1,50-1);//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.
}

//TIM1 PWM部分初始化 （PA9,PA10为usart1引脚）
//arr：自动重装值
//psc：时钟预分频数
// 通道配置：
//  |- CH1->PA8 -->X-->内环AIN1
//  |- CH2->PE11-->Y-->内环BIN1
//  |- CH3->PE13-->Y-->内环BIN2
//  |- CH4->PA11-->X-->内环AIN2
void Timer1_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);//使能PORTA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);//使能PORTE时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);//GPIOA8复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOE11复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//GPIOE13复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);//GPIOA11复用为定时器1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;//TIM1_CH1-->PA8,TIM1_CH4-->PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA8 PA11 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;//TIM1_CH2-->PE11,TIM1_CH3-->PE13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度100MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化PA8 PA11 
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频因子
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);//初始化定时器1
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//选择定时器模式(初始化输出比较,TIMx_CNT<TIM_CCRx的时候为有效电平)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性:TIM输出比较极性高(有效电平为高)
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);//初始化TIM1的输出比较通道1
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);//初始化TIM1的输出比较通道2
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);//初始化TIM1的输出比较通道3
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);//初始化TIM1的输出比较通道4
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC1预装载寄存器使能
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC2预装载寄存器使能
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC3预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC4预装载寄存器使能

	TIM_SetCompare1(TIM1,0);//调节通道一占空比
	TIM_SetCompare2(TIM1,0);//调节通道二占空比
	TIM_SetCompare3(TIM1,0);//调节通道三占空比
	TIM_SetCompare4(TIM1,0);//调节通道四占空比
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);// 使能PWM输出（高级定时器特有）
  TIM_ARRPreloadConfig(TIM1, ENABLE);// 使能TIM1的自动重加载寄存器（ARPE使能）
	TIM_Cmd(TIM1,ENABLE);//使能TIM1
}

//TIM8 PWM部分初始化 （PA9,PA10为usart1引脚）
//arr：自动重装值
//psc：时钟预分频数
// 通道配置：
//  |- CH1->PC6-->X-->外环AIN1
//  |- CH2->PC7-->Y-->外环BIN1
//  |- CH3->PC8-->Y-->外环BIN2
//  |- CH4->PC9-->X-->外环AIN2
void Timer8_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//TIM8时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);//使能PORTC时钟
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);//GPIOC6复用为定时器8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);//GPIOC7复用为定时器8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);//GPIOC8复用为定时器8
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);//GPIOC9复用为定时器8

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//TIM8_CH1-->PC6,TIM8_CH2-->PC7,TIM8_CH3-->PC8,TIM8_CH4-->PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化PC6 PC7 PC8 PC9  
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频因子
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);//初始化定时器8
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//选择定时器模式(初始化输出比较,TIMx_CNT<TIM_CCRx的时候为有效电平)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性:TIM输出比较极性高(有效电平为高)
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能
	TIM_OC1Init(TIM8,&TIM_OCInitStruct);//初始化TIM8的输出比较通道1
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);//初始化TIM8的输出比较通道2
	TIM_OC3Init(TIM8,&TIM_OCInitStruct);//初始化TIM8的输出比较通道3
	TIM_OC4Init(TIM8,&TIM_OCInitStruct);//初始化TIM8的输出比较通道4
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC1预装载寄存器使能
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC2预装载寄存器使能
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC3预装载寄存器使能
	TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);//OC4预装载寄存器使能

	TIM_SetCompare1(TIM8,0);//调节通道一占空比
	TIM_SetCompare2(TIM8,0);//调节通道二占空比
	TIM_SetCompare3(TIM8,0);//调节通道三占空比
	TIM_SetCompare4(TIM8,0);//调节通道四占空比
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);// 使能PWM输出（高级定时器特有）
  TIM_ARRPreloadConfig(TIM8, ENABLE);// 使能TIM8的自动重加载寄存器（ARPE使能）
	TIM_Cmd(TIM8,ENABLE);//使能TIM8
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void PWM_Load(motor* moto, int32_t pwmin)
{
	//限幅
	if(pwmin > moto->PWM_MAX){
		pwmin = moto->PWM_MAX;
	  } 
	else if(pwmin < -moto->PWM_MAX){
		pwmin = -moto->PWM_MAX;
	  }
	moto->pwm = pwmin;	
//  printf("          %d\r\n",pwmin);

  //正负号对应正反转,装载PWM值
	if(moto == &motor_A){
		if(moto->pwm > 0){
			TIM_SetCompare1(TIM1,8400);
			TIM_SetCompare4(TIM1,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // 添加了条件
			TIM_SetCompare4(TIM1,8400);
			TIM_SetCompare1(TIM1,(8400+moto->pwm));
		}
	}
	if(moto == &motor_B){
		if(moto->pwm > 0){
			TIM_SetCompare3(TIM1,8400);
			TIM_SetCompare2(TIM1,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // 添加了条件
			TIM_SetCompare2(TIM1,8400);
			TIM_SetCompare3(TIM1,(8400+moto->pwm));
		}
	}
	if(moto == &motor_C){
		if(moto->pwm > 0){
			TIM_SetCompare1(TIM8,8400);
			TIM_SetCompare2(TIM8,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // 添加了条件
			TIM_SetCompare2(TIM8,8400);
			TIM_SetCompare1(TIM8,(8400+moto->pwm));
		}
	}
	if(moto == &motor_D){
		if(moto->pwm > 0){
			TIM_SetCompare4(TIM8,8400);
			TIM_SetCompare3(TIM8,(8400-moto->pwm));
		}else if(moto->pwm < 0){  // 添加了条件
			TIM_SetCompare3(TIM8,8400);
			TIM_SetCompare4(TIM8,(8400+moto->pwm));
		}
	}
}
