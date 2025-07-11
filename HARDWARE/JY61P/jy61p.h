#ifndef __JY61P_H
#define __JY61P_H
#include "sys.h"

struct StcAngle//角度
{
    short Angle_S[3];  //将串口接收的角度数据存放在这里(角度还有经过计算)
    short T;          //时间戳
    float Angle[3]; //计算后的角度数据
};

struct StcGyro//角加速度
{
    short Gyro_S[3];
    short T;
    float Gyro[3];
};

struct StcAcc//加速度
{
    short Acc_S[3];
    short T;
    float Acc[3];
};
extern struct StcAngle SAngle;
extern struct StcGyro SGyro;
extern struct StcAcc SAcc;

void Get_JY61P(uint16_t Data);
			    
#endif
