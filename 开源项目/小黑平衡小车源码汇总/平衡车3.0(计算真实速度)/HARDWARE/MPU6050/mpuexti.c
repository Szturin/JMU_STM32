#include "mpuexti.h"
#include "sys.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "OLED.h"
#include "delay.h"
#include "pid.h"
#include "motor.h"
#include "timer.h"
#include "led.h"
extern int motor;            //���ʹ�ܱ�־���ֶ����ƣ�
extern int motor_flag;       //���ʹ�ܱ�־���Զ����ƣ�
extern float zhongzhi;       //��е��ֵ��roll������ֵ��
extern int velocity_calcu;   //�ٶ�����ֵ
extern float velocity_Rcalcu;//��ʵ�ٶ�����ֵ
extern float yaw_calcu;      //ת������ֵ
float pitch,roll,yaw;        //ŷ����
float measure,calcu;         //roll�ǲ���ֵ������ֵ 
int velocity_measure;        //�ٶȲ���ֵ
float velocity_Rmeasure;     //��ʵ�ٶȲ���ֵ
float yaw_measure;           //yaw�ǲ���ֵ
int PWM0,PWM1,PWM2;          //���������PWMֵ 
int tenms=0;                 //10ms�������ñ���ÿ��һ�δ������10ms


void MPU_exti_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				  //ʹ�ܸ��ù���ʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;								  //PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 					    //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12); //��PA12ӳ�䵽�ж���12
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	  	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	//��ռ���ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************

					�ⲿ�ж���12�������PID�㷨�ڴ˺�����

          ע�������ϸ�֤���ݽ��պ�PID�����ͬ��

***********************************************************/ 
void EXTI15_10_IRQHandler(void) //10ms�ж�
{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{ 	
		  if( (tenms++) %100==0)   LED1;             //ÿ����һ��
			else                     LED0;
		
	    //��1.ֱ����������
			measure = roll;                            //roll����ֵ
			calcu = zhongzhi;                          //roll����ֵ
			
		  //��2.�ٶȻ�������
		  //ת�ٲ���ֵ = �����ת��ƽ��ֵ��
			velocity_measure = ( read_encoder2() + read_encoder3() ) /2; 
			velocity_Rmeasure = (velocity_measure*0.0001365) /0.01;      //���ղ����������������ʵ�����ٶ�=·��/ʱ��
		  velocity_calcu = (int)( (velocity_Rcalcu*0.01) /0.0001365 ); //������ʵ�����ٶȼ�������������� 
		
			//��3.ת�򻷲�����                              
      yaw_measure = yaw;                         //yaw����ֵ  		
		
		
		  //PID���㣺ֱ���� + �ٶȻ� + ת�򻷣�
		  PWM0 = vertical_PID_value(measure,calcu) + velocity_PID_value(velocity_measure,velocity_calcu);
			PWM1 = PWM0 + turn_PID_value(yaw_measure,yaw_calcu); 
		  PWM2 = PWM0 - turn_PID_value(yaw_measure,yaw_calcu); 
			PWM_Xianfu(7000,&PWM1,&PWM2);              //PWM�޷�
			
			if(motor_flag && motor) SETPWM(PWM1,PWM2); //�����PWM
			else                    SETPWM(0,0);       //�رյ��		
	}
	
	EXTI_ClearITPendingBit(EXTI_Line12);           //���LINE0�ϵ��жϱ�־λ  
}
