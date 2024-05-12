#include <stm32f10x.h> 			//Device header
#include "PWM.h"
#include <Key.h>
#include <OLED.h>
#include <SERVO.h>

unsigned char KeyNum=0;
unsigned int Angle;

int main(void)
{
	OLED_Init();
	Servo_Init();
	Key_Init();//KEY没有初始化，导致Angle一直自动加
	Servo_SetAngle(90);
	OLED_ShowString(1,1,"Angle:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Angle += 30;
			if(Angle > 180 )
			{
				Angle = 0;
			}
		}
		Servo_SetAngle(Angle);
		OLED_ShowNum(1,7,Angle,3);
		OLED_ShowNum(2,7,KeyNum,1);
	}
}
