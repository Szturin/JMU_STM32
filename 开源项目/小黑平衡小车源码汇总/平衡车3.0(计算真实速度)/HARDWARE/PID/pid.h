#ifndef __PID_H
#define __PID_H

int vertical_PID_value(float measure,float calcu);               //ֱ����
int velocity_PID_value(int velocity_measure,int velocity_calcu); //�ٶȻ�
int turn_PID_value(float yaw_measure,float yaw_calcu);           //ת��
void I_xianfu(int max);                                          //pwm�޷�
#endif
