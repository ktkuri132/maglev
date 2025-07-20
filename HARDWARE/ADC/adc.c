#include "adc.h"
#include "delay.h"		 
#include <stdio.h>

// ����ȫ�ֱ����洢ADCֵ
uint16_t ADC_Values[5] = {0}; // �洢CH5/6/7��ת��ֵ
#define  ADC1_DR_ADDRESS         ((uint32_t)0x4001204C) //ADC1�����ַ

/*
  A5-->X
  A6-->Y
  A7-->Z
  B0-->XI
  B1-->YI
*/
/*=============================================��ʼ��ADC=============================================*/															   
void  Adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 1. ����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA\Bʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

    // 2. ����GPIOΪģ�����루ADC1ͨ��5-->PA5��ADC1ͨ��6-->PA6��ADC1ͨ��7-->PA7��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
    
    // ADC1ͨ��8-->PB0��ADC1ͨ��9-->PB1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  

    // 3. ��λADC1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

    // 4. ����ADCͨ������
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

    // 5. ����ADC1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
    ADC_InitStructure.ADC_NbrOfConversion = 5;//1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

    // 6. ����ADCת�����У�ͨ����ͨ��5��6��7��8��9��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);//����ADCͨ��ת��˳�򣬲���ʱ��15������(��߲���ʱ�������߾���)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_480Cycles);//����ADCͨ��ת��˳�򣬲���ʱ��15������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_480Cycles);//����ADCͨ��ת��˳�򣬲���ʱ��15������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_480Cycles);//����ADCͨ��ת��˳�򣬲���ʱ��15������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5, ADC_SampleTime_480Cycles);//����ADCͨ��ת��˳�򣬲���ʱ��15������

  
	// 7. ʹ��DMA����
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);// ʹ��ADC DMA

	// 8. ����ADC
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);  // �������ת��
	
}	

/*=============================================DMA���ã������Զ����ԭʼ�������飩=============================================*/															   
void DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DM2ʱ��ʹ��
	
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;//ѡ��DMAͨ��  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;//DMA2�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_Values;//DMA2�洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = 5;//�趨һ�δ�������ݸ��������ݴ�������
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���Ϊ16λ�����ֽڣ�
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢���ݳ���λ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ʹ��ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA_Priority_High;//�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //FIFOģʽ���� 
  //FIFOģʽ��ֹʱ��������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//ʹ��1/2FIFO��ֵ
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��DMA2 Stream0
  DMA_Cmd(DMA2_Stream0, ENABLE);  //����DMA����  
}

