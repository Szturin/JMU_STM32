#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <OLED.h>
#include "PWM.h"
#include "IC.h"

unsigned char Duty;

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	PWM_SetCompare1(Duty); //Duty =CCR/100
	PWM_SetPrescaler(7200-1); //Freq = 72M/(PSC+1)/100
	
	OLED_ShowString(1,1,"Freq:     HZ");
	OLED_ShowString(2,1,"Duty:  %");
	while(1)
	{
		PWM_SetCompare1(Duty); //Duty =CCR/100
		OLED_ShowNum(1,6,IC_GetFreq(),5);
		OLED_ShowNum(2,6,IC_GetDuty(),2);//为什么无法测量100占空比？
		OLED_ShowNum(2,10,Duty,2);
		Duty++;
		Delay_ms(1000);
	}
}

//频率测量的上下限？