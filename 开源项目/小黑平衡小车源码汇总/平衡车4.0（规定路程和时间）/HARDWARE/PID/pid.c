#include "pid.h"
extern float Kp,Ki,Kd;   //直立环参数
float err;               //此次误差
float last_err;          //上次误差
float err_sum=0;         //误差累加
float err_difference;    //误差的差值

extern float VKp,VKi;    //速度环参数
int velocity;            //速度偏差
float filt_velocity;     //滤波后的速度
float last_filt_velocity;//上一次的滤波后的速度
float velocity_sum=0;    //速度的累加

extern float TKp;        //转向环参数
float yaw_err;           //转向偏差


//直立环：
int vertical_PID_value(float measure,float calcu)
{

	err = measure - calcu;             //误差
	err_sum+=err;                      //误差的累加
	err_difference = err - last_err;   //误差的差值
	last_err = err;                    //此次误差记录为“上次误差”
	
	return Kp*err + Ki*err_sum + Kd*err_difference; //结果
}


//速度环：
int velocity_PID_value(int velocity_measure,int velocity_calcu)
{
	float a=0.3;                                           //滤波系数（反映滤波程度）
	velocity = velocity_measure - velocity_calcu;          //速度偏差
	filt_velocity = a*velocity + (1-a)*last_filt_velocity; //一阶速度滤波
	velocity_sum +=  filt_velocity;                        //速度的累加
	I_xianfu(3000);                                        //累加限幅
	last_filt_velocity = filt_velocity;                    //此次速度记录为“上次速度”

	return VKp*filt_velocity + VKi*velocity_sum;
}


//转向环：
int turn_PID_value(float yaw_measure,float yaw_calcu)
{
	yaw_err = yaw_measure - yaw_calcu; //转向偏差
	
	return TKp*yaw_err;
}


//I限幅：
void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}
