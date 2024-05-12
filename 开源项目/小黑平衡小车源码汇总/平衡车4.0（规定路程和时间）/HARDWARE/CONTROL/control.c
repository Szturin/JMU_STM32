//С�� qq1501451224 2021.7.13

#include "control.h"
extern float length;
extern int total_time;
float Vmax;      //���߶���
float K1,K2;     //���ٺͼ������ߵ�б��
int task_flag=0; //������ɱ�־  0δ���  1���


//������һʱ�̵������ٶȣ����� v-t ������ϳ����ģ���
float velocity_calculate(float current_time)
{
	float velocity;
	
	//�����߽�����ϣ�
	Vmax = length/(0.5*total_time);    //�ٶ����ֵ
	K1 = Vmax/(0.5*total_time);        //��������б��
	K2 = -K1;                          //��������б��
	
	if(current_time < 0.5*total_time)  //��Ϊ���ٵ�����
	{
		velocity = K1*current_time;
	}
	
	if(current_time >= 0.5*total_time) //��Ϊ���ٵ�����
	{
		velocity = K2*current_time - K2*total_time;
	}
	
	if(current_time > total_time)      //�������
	{
		velocity = 0;                    //ԭ����Ϣ
		task_flag = 1;                   //������ɱ�־
	}	
	
		
	return velocity;
}
