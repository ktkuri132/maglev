#include "usart3.h"	
#include "sys.h"
#include "jy61p.h"	

/*****************************************初始化UART3(JY61P)*****************************************/
//bound:波特率115200
void usart3_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;//串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;//串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;//串口中断配置结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//USART3:TX-->PB10(接RX), RX-->PB11
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //PB11复用为USART3
	
	//USART3 端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10,GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10，PB11

  //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
  USART_Cmd(USART3, ENABLE);  //使能串口3

//	//如下语句解决第1个字节无法正确发送出去的问题
//	USART_ClearFlag(USART3, USART_FLAG_TC);//清串口3发送标志
}



void USART3_IRQHandler(void)
{
    static int i = 0;
	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//检查是否接收到数据(接收到的数据必须是0xb3 0x5b结尾)
    {
			  uint16_t Data=(USART3->DR & (uint16_t)0x01FF);
        Get_JY61P(Data);
    }	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //清除标志位
}

