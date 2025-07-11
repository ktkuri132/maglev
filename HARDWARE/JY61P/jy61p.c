#include "jy61p.h"	
#include "sys.h"	
#include <string.h>

//定义结构体名
struct StcAngle SAngle;
struct StcGyro  SGyro;
struct StcAcc   SAcc;

/*****************************采集JY61P数据********************************/
void Get_JY61P(uint16_t Data)
{
    static u8 RX_Data[250];
    static u8 len = 0;
    RX_Data[len++]=Data;    //将收到的数据存入缓冲区中
    if (RX_Data[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头,0x51/0x52/0x53 分别表示加速度、角速度、角度数据
    {
        len=0;
        return;
    }
    if (len<11) return;//数据不满11个，则返回
    else
    {
     switch(RX_Data[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
     {  uint8_t j;
        case 0x51:      
                        memcpy(&SAcc,&RX_Data[2],8);
                        for(j = 0; j < 3; j++)
                            {
                                SAcc.Acc[j] = (float)SAcc.Acc_S[j]/32768*16;
                            }
                        break;
        case 0x52:
                        memcpy(&SGyro,&RX_Data[2],8);
                        for(j = 0; j < 3; j++)
                            {
                                SGyro.Gyro[j] = (float)SGyro.Gyro_S[j]/32768*2000;
                            }
                        break;
        case 0x53:
                        memcpy(&SAngle,&RX_Data[2],8);//角度数据（0x53） 包含 roll、pitch、yaw
                        for(j = 0; j < 3; j++)
                            {
                               SAngle.Angle[j] = (float)SAngle.Angle_S[j]/32768.00f*180;
                            }
                        break;
     }
     len=0;//清空缓存区
 }
}
