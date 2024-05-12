#include "dianji.h"
#include "motor.h"

void dianjioutput(int Voltage)
{
	if(Voltage>0)
	{
	    GPIO_SetBits(GPIOB, GPIO_Pin_14 );		     //�������ģ��AIN1�� PB14		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_15 );       //�������ģ��AIN2�� PB15	

		  GPIO_SetBits(GPIOB, GPIO_Pin_13 );			   //�������ģ��BIN1�� PB13		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_12 );       //�������ģ��BIN2�� PB12	
	}
	else
	{
		  GPIO_SetBits(GPIOB, GPIO_Pin_15 );		     //�������ģ��AIN1�� PB14			    
      GPIO_ResetBits(GPIOB, GPIO_Pin_14 );       //�������ģ��AIN2�� PB15	
  
		  GPIO_SetBits(GPIOB, GPIO_Pin_12 );			   //�������ģ��BIN1�� PB12		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 );       //�������ģ��BIN2�� PB13	
			
			Voltage=-Voltage;		
	}
	    TIM_SetCompare3(TIM2,Voltage);			       //TIM2�� Voltage�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
	    TIM_SetCompare4(TIM2,Voltage);			       //TIM3�� Voltage�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
}


