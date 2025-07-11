#ifndef TOF10120_H
#define TOF10120_H
#include <sys.h>
#include <usart.h>
#include <delay.h>


extern unsigned short int length_val;

void TOF10120_Init();
void TOF10120_GetData();
void TOF10120_USART_ReceiveData();

#endif