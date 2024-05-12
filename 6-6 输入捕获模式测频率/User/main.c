#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <OLED.h>
#include "PWM.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	PWM_SetCompare1(50); //Duty =CCR/100
	PWM_SetPrescaler(720-1); //Freq = 72M/(PSC+1)/100
	
	OLED_ShowString(1,1,"Freq:00000HZ");
	while(1)
	{
		OLED_ShowNum(1,6,IC_GetFreq(),5);
	}
}
