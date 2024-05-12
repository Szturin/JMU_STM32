#include <stm32f10x.h> 			//Device header
#include "PWM.h"
#include <Key.h>
#include <OLED.h>
#include <MOTOR.H>

unsigned char KeyNum=0;
int8_t Speed;
                                                                                                                  
unsigned char KeyNum;

int main(void)
{
	OLED_Init();
	Key_Init();//KEY没有初始化，导致Angle一直自动加
	Motor_PWM_Init();
	OLED_ShowString(1,1,"Speed:");
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			Speed=Speed+20;
			if(Speed==120){Speed=0;}
		}
		else if(KeyNum==2)
		{
			Speed=Speed-20;
			if(Speed==-120){Speed=0;}
		}
		OLED_ShowSignedNum(1,7,Speed,4);
		Motor_SpeedSet(Speed);
	}
}
