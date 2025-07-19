#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 

extern uint16_t ADC_Values[5]; // 存储CH5/6/7的转换值

void Adc_Init(void); 				//ADC通道初始化
void ADC_IRQHandler(void);// 中断服务程序
void DMA_Config(void);

#endif 



