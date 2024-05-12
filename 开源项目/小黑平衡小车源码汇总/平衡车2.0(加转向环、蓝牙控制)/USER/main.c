//�ڶ��棺�������ƿ���ת���ǰ����
//С�� 2021.7.12

#include <stm32f10x.h>
#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor.h"
extern float pitch,roll,yaw; //ŷ����
extern int velocity_measure; //ת�ٲ���ֵ
extern float velocity_sum;   //�ٶȵ��ۼ�
extern char bluetooth;       //����ָ��

int i=0;
float zhongzhi=0;            //��е��ֵ��roll������ֵ��
int motor_flag;              //���ʹ�ܱ�־���Զ����ƣ���1ʹ��  0ʧ��
int motor = 1;               //���ʹ�ܱ�־���ֶ����ƣ���1ʹ��  0ʧ��
int velocity_calcu = 0;      //�ٶ�����ֵ ���ڴ˶������ֱ�Ӹ��ģ�	
float yaw_calcu = 0;         //yaw������ֵ���ڴ˶������ֱ�Ӹ��ģ�

//ֱ��������:
float Kp=-420,Ki=0,Kd=-2000; 
//�ٶȻ�����:
float VKp=+190,VKi=0.95;   
//ת�򻷲���:
float TKp=80;        



int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2: 2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	OLED_Init();
  motor_init();                     //����� IO  pwm  ������ ��ʼ��
	MPU_Init();		                    //MPU6050��ʼ��	 
	DMP_Init();                       //DMP��ʼ��	
	usart2_init(9600);                //��������2��ʼ��
	MPU_exti_init();                  //mpu�ⲿ�жϳ�ʼ����һ��Ҫ�����
	
/************************************************************	
                     ��������Ӳ��׼�����
*************************************************************/ 
	
	while(1)
	{
		if(roll<-30 || roll>30)         //С�������Ѿ�����
		{
			motor_flag = 0;               //�رյ��
			velocity_sum = 0;             //�ٶȻ�����0
		}
		else motor_flag=1;              //�������
		
		
		OLED_ShowAngle(roll,yaw);       //��ʾŷ����
		OLED_ShowPWM(velocity_measure); //��ʾת��
	}
}
