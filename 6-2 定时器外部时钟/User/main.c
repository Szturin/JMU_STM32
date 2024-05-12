#include <stm32f10x.h> //Device header
#include <Delay.h>
#include <OLED.h>
#include <CountSensor.h>
#include <Encoder.h>
#include <Timer.h>

uint16_t Num=0;//定义一个16位的全局变量

int main(void)
{
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(2,1,"CNT:");
	while (1)
	{
		OLED_ShowNum(1,5,Num,5);//计时的值
		OLED_ShowNum(2,5,Timer_GetCounter(),5);//CNT计数器的值
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//获取中断标志位，参数：定时器2，检测更新标志位
	{
		Num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
	}
}
