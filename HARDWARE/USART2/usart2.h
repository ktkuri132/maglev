#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 
	
#define USART2_REC_LEN  			128  	//�����������ֽ��� 200
#define EN_USART2_RX 			    1	//ʹ�ܣ�1��/��ֹ��0������2����


extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart2_init(u32 bound);
void usart_send(USART_TypeDef* USARTx, uint8_t* data, uint8_t len);
void Usart2_SendByte(uint8_t Data);
void Usart2_SendString(char* str);

void Send_Data_To_USART2(char dat);
void USART2_IRQHandler(void);

#endif


