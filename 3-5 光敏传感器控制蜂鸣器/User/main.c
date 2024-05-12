#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <LED.h>
#include <Key.h>
#include <BUZZER.h>
#include <LightSensor.h>
uint8_t KeyNum;

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	while(1)
	{
		if(LightSensor_Get())
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
}
