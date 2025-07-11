#include "tof.h"
#include "usart2.h"
#include <stdint.h>

unsigned char rxbuf[16],rxempty, rxcnt,rxflag,waitflag;
unsigned short int length_val;
unsigned int timeout;

void TOF10120_Init(){
	Send_Data_To_USART2('s');
	Send_Data_To_USART2('5');
	Send_Data_To_USART2('-');
	Send_Data_To_USART2('0');
	Send_Data_To_USART2('#'); //ÉèÎª±»¶¯¶ÁÈ¡Ä£Ê½
	delay_ms(10);
	waitflag=1;
//	while(waitflag==1);
	waitflag=0; 
	delay_ms(1000);
	rxcnt=0;
}

void TOF10120_GetData(){
	//Ö»ÓÐÉèÎª±»¶¯¶ÁÈ¡Ä£Ê½£¬²Å·¢ËÍr6#ÃüÁî
	uint8_t i;
	Send_Data_To_USART2('r');
	Send_Data_To_USART2('6');
	Send_Data_To_USART2('#');//Ö÷¶¯¶ÁÈ¡¾àÀëÃüÁî
	timeout=10000;
	while((rxflag==0)&&(timeout--));
	//Èç¹ûÊÇÖ÷¶¯·¢ËÍÖ»ÐèÔÚÖ÷³ÌÐòÀï²éÑ¯rxflag±êÖ¾
	if(rxflag)//½ÓÊÕµ½1×éÓÐÐ§Êý¾Ý
	{
		for(i=0; i<rxcnt; i++)
		{
			if(rxbuf[i]=='m')
			{
				if(rxbuf[i+1]=='m')	//ASCIIÂë×ª»»Îª16½øÖÆÊý¾Ý£¬µ¥Î»mm
				{
					if((i>0)&&(rxbuf[i-1]>='0')&&(rxbuf[i-1]<='9'))
						length_val=rxbuf[i-1]-'0';
					if((i>1)&&(rxbuf[i-2]>='0')&&(rxbuf[i-2]<='9'))
						length_val+=(rxbuf[i-2]-'0')*10;
					if((i>2)&&(rxbuf[i-3]>='0')&&(rxbuf[i-3]<='9'))
						length_val+=(rxbuf[i-3]-'0')*100;
					if((i>3)&&(rxbuf[i-4]>='0')&&(rxbuf[i-4]<='9'))
						length_val+=(rxbuf[i-4]-'0')*1000;
					break;
				}
			}
		}
		rxflag = 0;
		rxcnt = 0;
	}
}

void TOF10120_USART_ReceiveData(){
	if(rxflag==0)
		{
			rxbuf[rxcnt++] = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
			if(rxcnt>2)
			{
				if(waitflag==1)
				{
					if((rxbuf[rxcnt-2]=='o')&&(rxbuf[rxcnt-1]=='k'))//串口接收距离数据格式ASCII码0mm~2000mm
					{
						waitflag=2;
					}
				}
				else
				{
					if((rxbuf[rxcnt-1]=='m')&&(rxbuf[rxcnt-2]=='m'))//串口接收距离数据格式ASCII码0mm~2000mm
					{
						rxflag=1;
					}
				}
			}
			else if(rxcnt>=16)
			{
				rxcnt=0;
			}
		}
		else
		{
			rxempty = USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		} 
	
}
