#include "usart2.h"	
#include "tof.h"	


////////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
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

#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	

/*****************************************��ʼ��UART2(������TOF10120)*****************************************/
//bound:������9600
void uart2_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;//���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;//���ڲ������ýṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;//�����ж����ýṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//USART2:TX-->PA2 , RX-->PA3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3����ΪUSART2
	
	//USART2 �˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

  //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
	USART_ClearFlag(USART2, USART_FLAG_TC);//�崮��2���ͱ�־
}


#endif	


void usart_send(USART_TypeDef* USARTx, uint8_t* data, uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++) // ѭ�����������ֽڷ�������
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); // �ȴ�ֱ���������ݼĴ���Ϊ�գ������Է�����һ���ֽڣ�

        USART_SendData(USARTx, data[i]); // �������ֽڷ��͵����ݼĴ���

        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);         // �ȴ�ֱ��������ɣ��������Ѿ������ͳ�ȥ��
    }
}

/**
 * ������:Usart2_SendByte
 * ����:����2���͵����ֽ�
 * ����:Data
 * ���:��
 */
void Usart2_SendByte(uint8_t Data)
{
	USART_SendData(USART2,Data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
}

/**
 * ������:Usart2_SendString
 * ����:����2�����ַ���
 * ����:*str
 * ���:��
 */
void Usart2_SendString(char* str)
{
	while(* str)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
			USART_SendData(USART2,* str++);
		}
}

/*=============================================��������غ���=============================================*/
void Send_Data_To_USART2(char dat)
{	
  while ((USART2->SR & USART_SR_TXE) == 0);  // �ȴ� TXE ��־��λ
  USART2->DR = dat;  
} 

//����2�жϴ�����
void USART2_IRQHandler(void) //����2ȫ���жϷ�����         	
{
	unsigned char Res;

	// ���USART2�Ľ����жϱ�־λ�Ƿ����ã����Ƿ���յ����ݣ�
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
	   TOF10120_USART_ReceiveData();
  }
} 
