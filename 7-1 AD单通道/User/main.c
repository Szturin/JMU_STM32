#include <stm32f10x.h> 			//Device header
#include "OLED.h"
#include "AD.h"
#include "Delay.h"

uint16_t ADValue;
float Voltage;
int main(void)
{
	OLED_Init();
	AD_Init1();
	OLED_ShowString(1,1,"ADValue");
	OLED_ShowString(2,1,"Voltage:0.00V");
	while(1)
	{
		ADValue = AD_GetValue1();
		Voltage = ADValue / 4095.0 *3.3;
		OLED_ShowNum(1,9,ADValue,4);
		OLED_ShowNum(2,9,Voltage,1);
		OLED_ShowNum(2,11,(uint16_t)(Voltage*100)%100,2);
		Delay_ms(100);
		//OLED_ShowNum(1,9,ADValue/10*10,4);//第一位置0
	}
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//获取中断标志位，参数：定时器2，检测更新标志位
	{
		Speed=Encoder_GetSpeed();
		number++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
	}
}
*/