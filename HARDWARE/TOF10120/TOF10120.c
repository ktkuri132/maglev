#include "TOF10120.h"
#include "delay.h"
 
#include "usart.h"
 
//��������
unsigned char devid;
unsigned int i2cread_interval;
unsigned char dirt_send_flag;
unsigned char dirt_detection_flag;
unsigned short int length_val;
unsigned short int length_aveval;
 
 
/*******************************************************************************
* Function Name  : UserI2c_Start
* Description    : sck stable in H,sda falling edge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Start(void)
{
    USERI2C_SDA_1;
    delay_us(5);
    USERI2C_SCL_1;
    delay_us(5);
    USERI2C_SDA_0;  //START:when CLK is high,DATA change form high to low
    delay_us(5);
    USERI2C_SCL_0;  //ǯסI2C���ߣ�׼�����ͻ��������
    delay_us(30);  //��30uS��ʱ
}
 
 
/*******************************************************************************
* Function Name  : UserI2c_Stop
* Description    : sck stable in H,sda rising edge
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Stop(void)
{
    USERI2C_SCL_0;
    USERI2C_SDA_0;//STOP:when CLK is high DATA change form low to high
    delay_us(5);
    USERI2C_SCL_1;
    delay_us(5);
    USERI2C_SDA_1;  //����I2C���߽����ź�
    delay_us(5);
 
    delay_us(30);   //��30uS��ʱ
}
 
/*******************************************************************************
* Function Name  : UserI2c_Wait_Ack
* Description    : the 9th clock pulse period wait ack
* Input          : None
* Output         : None
* Return         : =0��ack
*								 : =1��ack
*******************************************************************************/
unsigned char UserI2c_Wait_Ack(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned short int ucErrTime = 0;
    unsigned char RetValue;
 
    //SDA����Ϊ����
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //��������
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    USERI2C_SCL_0;
    delay_us(5);
    USERI2C_SCL_1;
 
    ucErrTime = 10000;
 
    while( ucErrTime-- > 0 )
    {
        if(USERI2C_READ_SDA )
        {
            RetValue = 0;
        }
        else
        {
            RetValue = 1;
            break;
        }
    }
 
    delay_us(1);
    USERI2C_SCL_0;//ʱ�����0
 
    //SDA�����
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //����IO������ٶ�Ϊ50MHz���ߵ͵�ƽ״̬����л�Ƶ��
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    USERI2C_SDA_0;
    delay_us(30);     //��30uS��ʱ
    return RetValue;
}
 
 
/*******************************************************************************
* Function Name  : useri2c_ack
* Description    : the 9th clock pulse period, the receiver pulls sda low to
* 							 : acknowledge the receipt of the eight data bits.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void useri2c_ack(void)
{
    USERI2C_SCL_0;
    USERI2C_SDA_0;
    delay_us(5);
    USERI2C_SCL_1;
    delay_us(5);
    USERI2C_SCL_0;
    delay_us(30);     //��30uS��ʱ
}
 
/*******************************************************************************
* Function Name  : useri2c_nack
* Description    : no acknowledge the receipt of the eight data bits.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void useri2c_nack(void)
{
    USERI2C_SCL_0;
    USERI2C_SDA_1;
    delay_us(5);
    USERI2C_SCL_1;
    delay_us(5);
    USERI2C_SCL_0;
    delay_us(30);     //��30uS��ʱ
}
 
/*******************************************************************************
* Function Name  : UserI2c_Send_Byte
* Description    : write one data to i2c bus
* Input          : txd-data
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Send_Byte(unsigned char txd)
{
    unsigned char t;
 
    USERI2C_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    delay_us(5);
 
    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 0)
            USERI2C_SDA_1;
        else
            USERI2C_SDA_0;
 
        txd <<= 1;
        delay_us(5);
        USERI2C_SCL_1;
        delay_us(5);
        USERI2C_SCL_0;
    }
}
 
 
/*******************************************************************************
* Function Name  : UserI2c_Read_Byte
* Description    : read one data from i2c bus
* Input          : None
* Output         : None
* Return         : receoved data
*******************************************************************************/
unsigned char UserI2c_Read_Byte(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i, receive = 0;
 
    //SDA����Ϊ����
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //��������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    USERI2C_SDA_1;
    USERI2C_SCL_0;
    delay_us(30);
 
    for(i = 0; i < 8; i++ )
    {
        receive <<= 1;
        USERI2C_SCL_1;
        delay_us(5);
 
        if(USERI2C_READ_SDA)
            receive++;
 
        USERI2C_SCL_0;
        delay_us(5);
    }
 
    USERI2C_SCL_0;
 
    //SDA�����
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    return receive;
}
/**************************************************************************************************************************/
 
/*******************************************************************************
* Function Name  :SensorWritenByte
* Description    :  Sensor read api.
* Input          : devid-�豸i2c��ַ
*                : *TXBuff-the address of the buffer which to write.
*                : SubAdd-the address read to
*                : Size-the buffer size
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SensorWritenByte(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size)
{
    unsigned char i = 0;
 
    UserI2c_Start();
 
    UserI2c_Send_Byte( Devid | 0x00 );
 
    if( 0 == UserI2c_Wait_Ack() )
    {
        UserI2c_Stop();
        return 0;
    }
 
    UserI2c_Send_Byte( SubAdd & 0xff );
 
    if( 0 == UserI2c_Wait_Ack() )
    {
        UserI2c_Stop();
        return 0;
    }
 
    for ( i = 0; i < Size; i++)
    {
        UserI2c_Send_Byte( TXBuff[Size - i - 1] );
 
        if( 0 == UserI2c_Wait_Ack() )
        {
            UserI2c_Stop();
            return 0;
        }
    }
 
    UserI2c_Stop();
 
    return 1;
}
 
 
/*******************************************************************************
* Function Name  : SensorReadnByte
* Description    :  Sensor read api.
* Input          : devid-�豸i2c��ַ
*                : *RXBuff-the address of the buffer which stores read content.
*                : SubAdd-the address read from
*                : Size-the buffer size
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SensorReadnByte(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size)
{
    unsigned char i = 0;
 
    UserI2c_Start();
 
//    UserI2c_Send_Byte( Devid | 0x00 ); //ʵʱ�����ַ
		UserI2c_Send_Byte( Devid | 0x04 );   //�˲������ַ
    if( 0 == UserI2c_Wait_Ack() )
    {
        UserI2c_Stop();
        return 0;
    }
 
    UserI2c_Send_Byte( SubAdd & 0xff );
 
    if( 0 == UserI2c_Wait_Ack() )
    {
        UserI2c_Stop();
        return 0;
    }
 
    UserI2c_Stop();
 
    UserI2c_Start();
    //UserI2c_Send_Byte( Devid | 0x01 ); //ʵʱ�����ַ
		UserI2c_Send_Byte( Devid | 0x05 );   //�˲������ַ
 
 
    if( 0 == UserI2c_Wait_Ack() )
    {
	//  UserI2c_Stop();
	//    return 0;
    }
 
    for ( i = 0; i < Size; i++)
    {
 
        RXBuff[Size - i - 1] = UserI2c_Read_Byte();
 
        if((i + 1) == Size)
            useri2c_nack();
        else
            useri2c_ack();
 
    }
 
    UserI2c_Stop();
 
    return 1;
}
 
 
/*******************************************************************************
* Function Name  : UserI2c_Init
* Description    : config i2c driver gpio
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(	I2C_SCL_CLK, ENABLE );
    RCC_AHB1PeriphClockCmd(	I2C_SDA_CLK, ENABLE );
 
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);


    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN); 	//SCL �����
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN); 	//SDA �����
/************************/
/**����TOF�ڲ��Ĵ���*****/
/**�������2022/6/1_dpt**/
/************************/
	devid=I2C_DEVID;       //����I2C�ӻ���ַ  Ĭ��164 0xA4
	
	dirt_send_flag=1;			 //=0������������ =1���ڻ���I2C������ȡ
	SensorWritenByte(devid,(unsigned char *)&dirt_send_flag, 0x09, 1);      //0x09���þ��뷢�ͷ�ʽ  1 ����ȥ��ȡ  0 ģ�鷢��
	delay_ms(100);
	
	dirt_detection_flag=0; //=0�˲�ֵ =1ʵʱֵ
	SensorWritenByte(devid,(unsigned char *)&dirt_detection_flag, 0x08, 1);  //0x08���� ��������ģʽ   1 ʵʱֵ 0�˲�ֵ
	delay_ms(100);		
	
}
 
/************************END OF FILE*************************/