#include "pid.h"
extern float Kp,Ki,Kd;   //ֱ��������
float err;               //�˴����
float last_err;          //�ϴ����
float err_sum=0;         //����ۼ�
float err_difference;    //���Ĳ�ֵ

extern float VKp,VKi;    //�ٶȻ�����
int velocity;            //�ٶ�ƫ��
float filt_velocity;     //�˲�����ٶ�
float last_filt_velocity;//��һ�ε��˲�����ٶ�
float velocity_sum=0;    //�ٶȵ��ۼ�

extern float TKp;        //ת�򻷲���
float yaw_err;           //ת��ƫ��


//ֱ������
int vertical_PID_value(float measure,float calcu)
{

	err = measure - calcu;             //���
	err_sum+=err;                      //�����ۼ�
	err_difference = err - last_err;   //���Ĳ�ֵ
	last_err = err;                    //�˴�����¼Ϊ���ϴ���
	
	return Kp*err + Ki*err_sum + Kd*err_difference; //���
}


//�ٶȻ���
int velocity_PID_value(int velocity_measure,int velocity_calcu)
{
	float a=0.3;                                           //�˲�ϵ������ӳ�˲��̶ȣ�
	velocity = velocity_measure - velocity_calcu;          //�ٶ�ƫ��
	filt_velocity = a*velocity + (1-a)*last_filt_velocity; //һ���ٶ��˲�
	velocity_sum +=  filt_velocity;                        //�ٶȵ��ۼ�
	I_xianfu(3000);                                        //�ۼ��޷�
	last_filt_velocity = filt_velocity;                    //�˴��ٶȼ�¼Ϊ���ϴ��ٶȡ�

	return VKp*filt_velocity + VKi*velocity_sum;
}


//ת�򻷣�
int turn_PID_value(float yaw_measure,float yaw_calcu)
{
	yaw_err = yaw_measure - yaw_calcu; //ת��ƫ��
	
	return TKp*yaw_err;
}


//I�޷���
void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}
