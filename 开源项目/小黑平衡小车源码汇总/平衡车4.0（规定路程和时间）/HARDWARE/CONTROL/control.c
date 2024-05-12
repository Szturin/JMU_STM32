//小黑 qq1501451224 2021.7.13

#include "control.h"
extern float length;
extern int total_time;
float Vmax;      //曲线顶点
float K1,K2;     //加速和减速曲线的斜率
int task_flag=0; //任务完成标志  0未完成  1完成


//计算任一时刻的理论速度（按照 v-t 曲线拟合出来的）：
float velocity_calculate(float current_time)
{
	float velocity;
	
	//对曲线进行拟合：
	Vmax = length/(0.5*total_time);    //速度最大值
	K1 = Vmax/(0.5*total_time);        //加速曲线斜率
	K2 = -K1;                          //减速曲线斜率
	
	if(current_time < 0.5*total_time)  //则为加速的曲线
	{
		velocity = K1*current_time;
	}
	
	if(current_time >= 0.5*total_time) //则为减速的曲线
	{
		velocity = K2*current_time - K2*total_time;
	}
	
	if(current_time > total_time)      //任务完成
	{
		velocity = 0;                    //原地休息
		task_flag = 1;                   //任务完成标志
	}	
	
		
	return velocity;
}
