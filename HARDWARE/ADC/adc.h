#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 

extern uint16_t ADC_Values[5]; // �洢CH5/6/7��ת��ֵ

void Adc_Init(void); 				//ADCͨ����ʼ��
void ADC_IRQHandler(void);// �жϷ������
void DMA_Config(void);

#endif 



