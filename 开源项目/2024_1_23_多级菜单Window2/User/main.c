#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Timer.h"
#include "Encoder.h"
#include "Key.h"
#include "OLED.h"
#include "Menu.h"

int Power_Off(void); // 关机

int main(void)
{
	Timer_Init();
	OLED_Init();
	Encoder_Init();
	Key_Init();

	uint8_t count = 0;
	while (1)
	{
		// Delay_ms(1000);
		OLED_Clear();

		//Menu_ShowWallpaper(Win11Wallpaper);
		
		//Menu_Showkunkun();
		OLED_ShowImage(0,0,64, 64, kunkun[count++/10]);
		OLED_ShowImage(64,0,64, 64, kunkun[count++/10]);
		count %= 130;
		
		
		if(Menu_Start(0) == 0)
		{
			if (Menu_EnterEvent())
			{
				Menu_Start(1);
			}
			if (Menu_BackEvent())
			{
				Power_Off();
			}
		}
		OLED_Update();
	}
}

int Power_Off(void) // 关机
{
	/*关闭外设区*******/
	OLED_Clear();
	OLED_Update();
	/*******关闭外设区*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // Enables or disables the Low Speed APB (APB1) peripheral clock.
	PWR_WakeUpPinCmd(ENABLE);

	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); // SIOP模式
	SystemInit();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE); // Enables or disables the Low Speed APB (APB1) peripheral clock.
	PWR_WakeUpPinCmd(DISABLE);

	Key_Reset_All();
	Key_Reset_All();

	return 1;
}
