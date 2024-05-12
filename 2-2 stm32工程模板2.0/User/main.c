#include <stm32f10x.h> 			//Device header

int main(void)
{
	while(1)
	{
		
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