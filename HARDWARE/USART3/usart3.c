#include "usart3.h"	
#include "sys.h"
#include "jy61p.h"	

/*****************************************��ʼ��UART3(JY61P)*****************************************/
//bound:������115200
void usart3_init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;//���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;//���ڲ������ýṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;//�����ж����ýṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//USART3:TX-->PB10(��RX), RX-->PB11
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //PB11����ΪUSART3
	
	//USART3 �˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10,GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10��PB11

  //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3

//	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
//	USART_ClearFlag(USART3, USART_FLAG_TC);//�崮��3���ͱ�־
}



void USART3_IRQHandler(void)
{
    static int i = 0;
	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//����Ƿ���յ�����(���յ������ݱ�����0xb3 0x5b��β)
    {
			  uint16_t Data=(USART3->DR & (uint16_t)0x01FF);
        Get_JY61P(Data);
    }	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //�����־λ
}

