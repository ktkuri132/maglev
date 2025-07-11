#include "led.h" 
#include "delay.h"

//初始化PD0,PD1,PD2为输出口,并使能时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟

  //GPIOD0,D1,D2初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;//LED对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
}


/************************下面是通过直接操作库函数的方式实现IO控制************************/
// 控制LED0
void LED0_Flash(uint16_t ms) 
{
    GPIO_SetBits(GPIOD, GPIO_Pin_4); // PD4置高，点亮LED0
    delay_ms(ms);                  // 延时
    GPIO_ResetBits(GPIOD, GPIO_Pin_4); // PD4置低，熄灭LED0
}

// 控制LED1
void LED1_Flash(uint16_t ms) 
{
    GPIO_SetBits(GPIOD, GPIO_Pin_5); // PD5置高，点亮LED1
    delay_ms(ms);                  // 延时
    GPIO_ResetBits(GPIOD, GPIO_Pin_5); // PD5置低，熄灭LED1
}

// 控制LED2
void LED2_Flash(uint16_t ms)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_6); // PD6置高，点亮LED2
    delay_ms(ms);                  // 延时
    GPIO_ResetBits(GPIOD, GPIO_Pin_6); // PD6置低，熄灭LED2
}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************

void LED0_Flash(uint16_t ms)  
{
    LED_red=1; // PD0置高，点亮LED0
    delay_ms(ms);                  // 延时
    LED_red=0; // PD0置低，熄灭LED0
}

// 控制LED1
void LED1_Flash(uint16_t ms) 
{
    LED_green=1; // PD1置高，点亮LED1
    delay_ms(ms);                  // 延时
    LED_green=0; // PD1置低，熄灭LED1
}

// 控制LED2
void LED2_Flash(uint16_t ms)
{
    LED_blue=1; // PD2置高，点亮LED2
    delay_ms(ms);                  // 延时
    LED_blue=0; // PD2置低，熄灭LED2
}

**************************************************************************************************
 **/	


