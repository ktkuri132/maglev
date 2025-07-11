#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 
	
#define USART2_REC_LEN  			128  	//定义最大接收字节数 200
#define EN_USART2_RX 			    1	//使能（1）/禁止（0）串口2接收


extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	


//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
void usart_send(USART_TypeDef* USARTx, uint8_t* data, uint8_t len);
void Usart2_SendByte(uint8_t Data);
void Usart2_SendString(char* str);

void Send_Data_To_USART2(char dat);
void USART2_IRQHandler(void);

#endif


