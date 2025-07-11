#include "usart2.h"	
#include "tof.h"	


////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 
void USART2_SendData(u8 data)
{
	while((USART2->SR & 0X40) == 0);
	USART2->DR = data;
}

void USART2_SendString(u8 *DAT, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART2_SendData(*DAT++);
	}
}

#if EN_USART2_RX   //如果使能了接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	

/*****************************************初始化UART2(激光测距TOF10120)*****************************************/
//bound:波特率9600
void uart2_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;//串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;//串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;//串口中断配置结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//USART2:TX-->PA2 , RX-->PA3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3复用为USART2
	
	//USART2 端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

  //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
  USART_Cmd(USART2, ENABLE);  //使能串口2

	//如下语句解决第1个字节无法正确发送出去的问题
	USART_ClearFlag(USART2, USART_FLAG_TC);//清串口2发送标志
}


#endif	


void usart_send(USART_TypeDef* USARTx, uint8_t* data, uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++) // 循环，用于逐字节发送数据
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); // 等待直到发送数据寄存器为空（即可以发送下一个字节）

        USART_SendData(USARTx, data[i]); // 把数据字节发送到数据寄存器

        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);         // 等待直到传输完成（即数据已经被发送出去）
    }
}

/**
 * 函数名:Usart2_SendByte
 * 描述:串口2发送单个字节
 * 输入:Data
 * 输出:无
 */
void Usart2_SendByte(uint8_t Data)
{
	USART_SendData(USART2,Data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
}

/**
 * 函数名:Usart2_SendString
 * 描述:串口2发送字符串
 * 输入:*str
 * 输出:无
 */
void Usart2_SendString(char* str)
{
	while(* str)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
			USART_SendData(USART2,* str++);
		}
}

/*=============================================激光测距相关函数=============================================*/
void Send_Data_To_USART2(char dat)
{	
  while ((USART2->SR & USART_SR_TXE) == 0);  // 等待 TXE 标志置位
  USART2->DR = dat;  
} 

//串口2中断处理函数
void USART2_IRQHandler(void) //串口2全局中断服务函数         	
{
	unsigned char Res;

	// 检查USART2的接收中断标志位是否被设置（即是否接收到数据）
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
	   TOF10120_USART_ReceiveData();
  }
} 
