#ifndef __LED_H
#define __LED_H
#include "sys.h"


//LED�˿ڶ���
#define LED_red   PDout(0)	//��ɫ
#define LED_green PDout(1)	//��ɫ	 
#define LED_blue  PDout(2)	//��ɫ	

void LED_Init(void);//��ʼ��

void LED0_Flash(uint16_t ms);
void LED1_Flash(uint16_t ms);
void LED2_Flash(uint16_t ms);

#endif
