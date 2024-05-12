#ifndef __PID_H
#define __PID_H

int vertical_PID_value(float measure,float calcu);               //直立环
int velocity_PID_value(int velocity_measure,int velocity_calcu); //速度环
int turn_PID_value(float yaw_measure,float yaw_calcu);           //转向环
void I_xianfu(int max);                                          //pwm限幅
#endif
