#include "adc.h"
#include "delay.h"		 
#include <stdio.h>

// 定义全局变量存储ADC值
uint16_t ADC_Values[5] = {0}; // 存储CH5/6/7的转换值
#define  ADC1_DR_ADDRESS         ((uint32_t)0x4001204C) //ADC1外设地址

/*
  A5-->X
  A6-->Y
  A7-->Z
  B0-->XI
  B1-->YI
*/
/*=============================================初始化ADC=============================================*/															   
void  Adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 1. 启用时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA\B时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

    // 2. 配置GPIO为模拟输入（ADC1通道5-->PA5、ADC1通道6-->PA6、ADC1通道7-->PA7）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
    
    // ADC1通道8-->PB0、ADC1通道9-->PB1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  

    // 3. 复位ADC1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

    // 4. 配置ADC通用设置
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化

    // 5. 配置ADC1
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
    ADC_InitStructure.ADC_NbrOfConversion = 5;//1个转换在规则序列中 也就是只转换规则序列1 
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化

    // 6. 配置ADC转换序列（通道：通道5、6、7、8、9）
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);//配置ADC通道转换顺序，采样时间15个周期(提高采样时间可以提高精度)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_480Cycles);//配置ADC通道转换顺序，采样时间15个周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_480Cycles);//配置ADC通道转换顺序，采样时间15个周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_480Cycles);//配置ADC通道转换顺序，采样时间15个周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5, ADC_SampleTime_480Cycles);//配置ADC通道转换顺序，采样时间15个周期

  
	// 7. 使能DMA请求
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);// 使能ADC DMA

	// 8. 启动ADC
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);  // 启动软件转换
	
}	

/*=============================================DMA配置（用于自动填充原始采样数组）=============================================*/															   
void DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DM2时钟使能
	
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;//选择DMA通道  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;//DMA2外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_Values;//DMA2存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
  DMA_InitStructure.DMA_BufferSize = 5;//设定一次传输的数据个数（数据传输量）
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度为16位（半字节）
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储数据长度位16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//使用循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA_Priority_High;//高优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //FIFO模式开启 
  //FIFO模式禁止时不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//使用1/2FIFO阀值
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA2 Stream0
  DMA_Cmd(DMA2_Stream0, ENABLE);  //开启DMA传输  
}

