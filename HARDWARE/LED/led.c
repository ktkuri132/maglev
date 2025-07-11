#include "led.h" 
#include "delay.h"

//��ʼ��PD0,PD1,PD2Ϊ�����,��ʹ��ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��

  //GPIOD0,D1,D2��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;//LED��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
}


/************************������ͨ��ֱ�Ӳ����⺯���ķ�ʽʵ��IO����************************/
// ����LED0
void LED0_Flash(uint16_t ms) 
{
    GPIO_SetBits(GPIOD, GPIO_Pin_4); // PD4�øߣ�����LED0
    delay_ms(ms);                  // ��ʱ
    GPIO_ResetBits(GPIOD, GPIO_Pin_4); // PD4�õͣ�Ϩ��LED0
}

// ����LED1
void LED1_Flash(uint16_t ms) 
{
    GPIO_SetBits(GPIOD, GPIO_Pin_5); // PD5�øߣ�����LED1
    delay_ms(ms);                  // ��ʱ
    GPIO_ResetBits(GPIOD, GPIO_Pin_5); // PD5�õͣ�Ϩ��LED1
}

// ����LED2
void LED2_Flash(uint16_t ms)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_6); // PD6�øߣ�����LED2
    delay_ms(ms);                  // ��ʱ
    GPIO_ResetBits(GPIOD, GPIO_Pin_6); // PD6�õͣ�Ϩ��LED2
}


/**
*******************����ע�͵��Ĵ�����ͨ�� λ�� ����ʵ��IO�ڿ���**************************************

void LED0_Flash(uint16_t ms)  
{
    LED_red=1; // PD0�øߣ�����LED0
    delay_ms(ms);                  // ��ʱ
    LED_red=0; // PD0�õͣ�Ϩ��LED0
}

// ����LED1
void LED1_Flash(uint16_t ms) 
{
    LED_green=1; // PD1�øߣ�����LED1
    delay_ms(ms);                  // ��ʱ
    LED_green=0; // PD1�õͣ�Ϩ��LED1
}

// ����LED2
void LED2_Flash(uint16_t ms)
{
    LED_blue=1; // PD2�øߣ�����LED2
    delay_ms(ms);                  // ��ʱ
    LED_blue=0; // PD2�õͣ�Ϩ��LED2
}

**************************************************************************************************
 **/	


