#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <OLED.h>
#include "Encoder.h"
#include "Timer.h"

int16_t Speed;
unsigned int number;
int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"Speed:");

	while(1)
	{
		OLED_ShowSignedNum(2,7,number,5);
		OLED_ShowSignedNum(1,7,Speed,5);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//��ȡ�жϱ�־λ����������ʱ��2�������±�־λ
	{
		Speed=Encoder_GetSpeed();
		number++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����־λ
	}
}
