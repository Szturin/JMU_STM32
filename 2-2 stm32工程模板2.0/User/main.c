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
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//��ȡ�жϱ�־λ����������ʱ��2�������±�־λ
	{
		Speed=Encoder_GetSpeed();
		number++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����־λ
	}
}
*/