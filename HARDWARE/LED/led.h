#ifndef __LED_H
#define __LED_H
#include "sys.h"


//LED端口定义
#define LED_red   PDout(0)	//红色
#define LED_green PDout(1)	//绿色	 
#define LED_blue  PDout(2)	//蓝色	

void LED_Init(void);//初始化

void LED0_Flash(uint16_t ms);
void LED1_Flash(uint16_t ms);
void LED2_Flash(uint16_t ms);

#endif
