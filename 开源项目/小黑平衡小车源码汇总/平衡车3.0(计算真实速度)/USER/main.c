//第三版：可以精确控制真实速度
//轮子周长：0.213m  一周编码器脉冲数=1560  --> 一个脉冲 = 0.0001365 m
//小黑 2021.7.13

#include <stm32f10x.h>
#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor.h"
#include "led.h"
extern float pitch,roll,yaw;   //欧拉角
extern int velocity_measure;   //速度测量值
extern float velocity_Rmeasure;//真实速度测量值
extern float velocity_sum;     //速度的累加
extern char bluetooth;         //蓝牙指令
extern int tenms;              //10ms计数，该变量每加一次代表过了10ms

int i=0;
float zhongzhi=0;              //机械中值（roll角理论值）
int motor_flag;                //电机使能标志（自动控制）：1使能  0失能
int motor = 1;                 //电机使能标志（手动控制）：1使能  0失能
int velocity_calcu;            //速度(脉冲数)理论值（在此定义便于直接更改）	
float velocity_Rcalcu = -0.35; //真实速度理论值m/s （在此定义便于直接更改）
float yaw_calcu = 0;           //yaw角理论值       （在此定义便于直接更改）

//直立环参数:
float Kp=-420,Ki=0,Kd=-2000; 
//速度环参数:
float VKp=+190,VKi=0.95;   
//转向环参数:
float TKp=80;        



int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2: 2位抢占优先级，2位响应优先级
	delay_init();                           //延时初始化
	OLED_Init();                            //OLED初始化
	LED_Init();                             //led灯初始化
  motor_init();                           //电机的 IO  pwm  编码器 初始化
	MPU_Init();		                          //MPU6050初始化	 
	DMP_Init();                             //DMP初始化	
	usart2_init(9600);                      //蓝牙串口2初始化
	MPU_exti_init();                        //mpu外部中断初始化（一定要放最后）
	
/************************************************************	
                     到此所有硬件准备完毕
*************************************************************/ 
	
	while(1)
	{
		if(roll<-30 || roll>30)               //小车可能已经倒了
		{
			motor_flag = 0;                     //关闭电机
			velocity_sum = 0;                   //速度积分清0
			tenms=0;                            //时间清零
		}
		else motor_flag=1;                    //开启电机
		
		
		OLED_ShowAngle(roll,yaw);             //显示欧拉角
		OLED_ShowVelocity(velocity_Rmeasure); //显示转速（脉冲计数）
	}
}
