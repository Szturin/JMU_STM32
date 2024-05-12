//���İ棺ʵ���˹涨ʱ������涨·��
//�����ܳ���0.213m  һ�ܱ�����������=1560  --> һ������ = 0.0001365 m
//С�� qq1501451224 2021.7.13

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
extern float pitch,roll,yaw;   //ŷ����
extern int velocity_measure;   //�ٶȲ���ֵ
extern float velocity_Rmeasure;//��ʵ�ٶȲ���ֵ
extern float velocity_sum;     //�ٶȵ��ۼ�
extern char bluetooth;         //����ָ��
extern int tenms;              //10ms�������ñ���ÿ��һ�δ������10ms
extern int task_flag;          //����ʼ/��ɱ�־

int i=0;
float zhongzhi=0;              //��е��ֵ��roll������ֵ��
int motor_flag=1;              //���ʹ�ܱ�־���Զ����ƣ���1ʹ��  0ʧ��
int motor = 1;                 //���ʹ�ܱ�־���ֶ����ƣ���1ʹ��  0ʧ��
int velocity_calcu;            //�ٶ�(������)����ֵ
float velocity_Rcalcu = 0;     //��ʵ�ٶȳ�ʼ����ֵm/s
float yaw_calcu = 0;           //yaw������ֵ       ���ڴ˶������ֱ�Ӹ��ģ�

//ֱ��������:
float Kp=-420,Ki=0,Kd=-2000; 
//�ٶȻ�����:
float VKp=+190,VKi=0.95;   
//ת�򻷲���:
float TKp=80;        

//����涨·�̣��ף���ʱ�䣨�룩:
float length=2;     
int total_time=15;  
float velocity_correction=0.0073; //�ٶ�������������������ٶ��趨̫С����������С����������


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2: 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();        //��ʱ��ʼ��
	OLED_Init();         //OLED��ʼ��
	LED_Init();          //led�Ƴ�ʼ��
  motor_init();        //����� IO  pwm  ������ ��ʼ��
	MPU_Init();		       //MPU6050��ʼ��	 
	DMP_Init();          //DMP��ʼ��	
	//usart2_init(9600); //openmv����2��ʼ��
	MPU_exti_init();     //mpu�ⲿ�жϳ�ʼ����һ��Ҫ�����
	
/************************************************************	
                     ��������Ӳ��׼�����
*************************************************************/ 
	
	while(1)
	{
		//��1.������ɽ�����ʾ��
		if(tenms<300) OLED_ShowStr(0,0,"          ",1);    //����������û�п�ʼ
		if(tenms>300)                                      //���������ʼ
		{
			if(task_flag==0) 
			{				
				OLED_ShowStr(0,0,"Start!",1);
				OLED_ShowNum(48,0,(int)( (tenms-300) /100),1); //��ʾ�������˶೤ʱ��
			}
			if(task_flag==1)                                 //���������
			{
				OLED_ShowStr(0,0,"Finish!",1);
				OLED_ShowNum(48,0,total_time,1);				
			}
		}	
				
		//��2.С��״̬��ʾ��		
		OLED_ShowAngle(roll,yaw);                          //��ʾŷ����
		OLED_ShowVelocity(velocity_Rmeasure);              //��ʾת��
		
		//��3.��ȫװ�á�
		if(roll<-30 || roll>30)                            //С�������Ѿ����ˣ����ǡ���λ��
		{
			motor_flag = 0;                                  //�رյ��
			velocity_sum = 0;                                //�ٶȻ�����0
			tenms = 0;                                       //ʱ������
			task_flag = 0;                                   //����δ��ɱ�־
		}
		else motor_flag=1;                                 //�������
	}
}
