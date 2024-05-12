#include "dianji.h"
#include "motor.h"

void dianjioutput(int Voltage)
{
	if(Voltage>0)
	{
	    GPIO_SetBits(GPIOB, GPIO_Pin_14 );		     //电机控制模块AIN1端 PB14		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_15 );       //电机控制模块AIN2端 PB15	

		  GPIO_SetBits(GPIOB, GPIO_Pin_13 );			   //电机控制模块BIN1端 PB13		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_12 );       //电机控制模块BIN2端 PB12	
	}
	else
	{
		  GPIO_SetBits(GPIOB, GPIO_Pin_15 );		     //电机控制模块AIN1端 PB14			    
      GPIO_ResetBits(GPIOB, GPIO_Pin_14 );       //电机控制模块AIN2端 PB15	
  
		  GPIO_SetBits(GPIOB, GPIO_Pin_12 );			   //电机控制模块BIN1端 PB12		    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 );       //电机控制模块BIN2端 PB13	
			
			Voltage=-Voltage;		
	}
	    TIM_SetCompare3(TIM2,Voltage);			       //TIM2与 Voltage对比，不相同则翻转波形，调节PWM占空比
	    TIM_SetCompare4(TIM2,Voltage);			       //TIM3与 Voltage对比，不相同则翻转波形，调节PWM占空比
}


