#ifndef __JY61P_H
#define __JY61P_H
#include "sys.h"

struct StcAngle//�Ƕ�
{
    short Angle_S[3];  //�����ڽ��յĽǶ����ݴ��������(�ǶȻ��о�������)
    short T;          //ʱ���
    float Angle[3]; //�����ĽǶ�����
};

struct StcGyro//�Ǽ��ٶ�
{
    short Gyro_S[3];
    short T;
    float Gyro[3];
};

struct StcAcc//���ٶ�
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
