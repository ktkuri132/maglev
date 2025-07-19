#ifndef __KALMAN_H
#define __KALMAN_H

typedef struct 
{
    float Last_P;//�ϴι���Э���� ������Ϊ0 ! ! ! ! ! 
    float Now_P;//��ǰ����Э����
    float out;//�������˲������
    float Kg;//����������
    float Q;//��������Э����
    float R;//�۲�����Э����
	  float source;
}Kalman;
extern Kalman kfp_x,kfp_y,kfp_z;


void Kalman_Init(Kalman *kalman);
float KalmanFilter(Kalman *kfp,float input);

#endif
