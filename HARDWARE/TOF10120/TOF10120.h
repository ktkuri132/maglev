#ifndef __TOF_H
#define __TOF_H
#include "sys.h"
 
//   绿线SCL —— PB12
//   蓝线SDA —— PB13
 
 
//管脚宏定义
#define I2C_SCL_PIN          	GPIO_Pin_6
#define I2C_SCL_PORT    		GPIOB
#define I2C_SCL_CLK       		RCC_AHB1Periph_GPIOB
 
#define I2C_SDA_PIN             GPIO_Pin_7
#define I2C_SDA_PORT          	GPIOB
#define I2C_SDA_CLK           	RCC_AHB1Periph_GPIOB
 
 
//IO操作函数
#define USERI2C_SCL_1   	GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define USERI2C_SCL_0   	GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define USERI2C_SDA_1   	GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define USERI2C_SDA_0   	GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)
 
#define USERI2C_READ_SDA  (GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN))
 
#define I2C_DEVID    			164//0XA4
 
 
//函数声明
void UserI2c_Start(void);
 
void UserI2c_Init(void);
 
unsigned char SensorWritenByte(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size);
unsigned char SensorReadnByte(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size);
 
 
#endif