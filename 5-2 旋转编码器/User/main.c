#include <stm32f10x.h> //Device header
#include <Delay.h>
#include <OLED.h>
#include <CountSensor.h>
#include <Encoder.h>

int n;
int main(void)
{
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1, 1, "Encoder_Count:");
	while (1)
	{
		n += Encoder_Get_Change_value();
		OLED_ShowSignedNum(2, 1, n, 5);
	}
}
