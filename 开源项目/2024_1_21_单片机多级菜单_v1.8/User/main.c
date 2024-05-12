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
	
	while (1)
	{
		Menu_RunMainMenu();

		Power_Off();
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

