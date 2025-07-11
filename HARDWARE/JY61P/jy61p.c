#include "jy61p.h"	
#include "sys.h"	
#include <string.h>

//����ṹ����
struct StcAngle SAngle;
struct StcGyro  SGyro;
struct StcAcc   SAcc;

/*****************************�ɼ�JY61P����********************************/
void Get_JY61P(uint16_t Data)
{
    static u8 RX_Data[250];
    static u8 len = 0;
    RX_Data[len++]=Data;    //���յ������ݴ��뻺������
    if (RX_Data[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ,0x51/0x52/0x53 �ֱ��ʾ���ٶȡ����ٶȡ��Ƕ�����
    {
        len=0;
        return;
    }
    if (len<11) return;//���ݲ���11�����򷵻�
    else
    {
     switch(RX_Data[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
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
                        memcpy(&SAngle,&RX_Data[2],8);//�Ƕ����ݣ�0x53�� ���� roll��pitch��yaw
                        for(j = 0; j < 3; j++)
                            {
                               SAngle.Angle[j] = (float)SAngle.Angle_S[j]/32768.00f*180;
                            }
                        break;
     }
     len=0;//��ջ�����
 }
}
