#include "TOF10120.h"
#include "delay.h"
 
#include "usart.h"
 
//变量定义
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
    USERI2C_SCL_0;  //钳住I2C总线，准备发送或接收数据
    delay_us(30);  //加30uS延时
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
    USERI2C_SDA_1;  //发送I2C总线结束信号
    delay_us(5);
 
    delay_us(30);   //加30uS延时
}
 
/*******************************************************************************
* Function Name  : UserI2c_Wait_Ack
* Description    : the 9th clock pulse period wait ack
* Input          : None
* Output         : None
* Return         : =0有ack
*								 : =1无ack
*******************************************************************************/
unsigned char UserI2c_Wait_Ack(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned short int ucErrTime = 0;
    unsigned char RetValue;
 
    //SDA设置为输入
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //上拉输入
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
    USERI2C_SCL_0;//时钟输出0
 
    //SDA线输出
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //配置IO的输出速度为50MHz，高低电平状态最高切换频率
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    USERI2C_SDA_0;
    delay_us(30);     //加30uS延时
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
    delay_us(30);     //加30uS延时
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
    delay_us(30);     //加30uS延时
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
 
    USERI2C_SCL_0;//拉低时钟开始数据传输
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
 
    //SDA设置为输入
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //上拉输入
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
 
    //SDA线输出
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
 
    return receive;
}
/**************************************************************************************************************************/
 
/*******************************************************************************
* Function Name  :SensorWritenByte
* Description    :  Sensor read api.
* Input          : devid-设备i2c地址
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
* Input          : devid-设备i2c地址
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
 
//    UserI2c_Send_Byte( Devid | 0x00 ); //实时距离地址
		UserI2c_Send_Byte( Devid | 0x04 );   //滤波距离地址
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
    //UserI2c_Send_Byte( Devid | 0x01 ); //实时距离地址
		UserI2c_Send_Byte( Devid | 0x05 );   //滤波距离地址
 
 
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);


    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN); 	//SCL 输出高
    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN); 	//SDA 输出高
/************************/
/**配置TOF内部寄存器*****/
/**代码改于2022/6/1_dpt**/
/************************/
	devid=I2C_DEVID;       //设置I2C从机地址  默认164 0xA4
	
	dirt_send_flag=1;			 //=0串口主动发送 =1串口或者I2C被动读取
	SensorWritenByte(devid,(unsigned char *)&dirt_send_flag, 0x09, 1);      //0x09设置距离发送方式  1 主机去读取  0 模块发送
	delay_ms(100);
	
	dirt_detection_flag=0; //=0滤波值 =1实时值
	SensorWritenByte(devid,(unsigned char *)&dirt_detection_flag, 0x08, 1);  //0x08设置 距离数据模式   1 实时值 0滤波值
	delay_ms(100);		
	
}
 
/************************END OF FILE*************************/