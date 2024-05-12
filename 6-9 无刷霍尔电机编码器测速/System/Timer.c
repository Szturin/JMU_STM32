#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��
	
	/*ʱ����Ԫ��ʼ������*/
	TIM_InternalClockConfig(TIM2);//��ʱ���ϵ��Ĭ��ʹ���ڲ�ʱ��,�����Ҳ���Բ�д
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	/*ʱ����Ԫ�ؼ��Ĵ���*/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 7200 - 1;//ARR �� ��72MHZ����7200��Ƶ Ҳ����10MHZ
	TIM_TimeBaseInitStructure.TIM_Period= 10000 - 1;//PSC����������װ��ֵ��10000������,����ARR��PSC���Կ��Ƽ���Ƶ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ���Ӵ��ڣ����ﲻ��Ҫ��
	
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	//1-->ע�⣺�����жϺ͸����¼�ͬʱ��������ʼ�������̽����ж�
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//2-->�ֶ������жϱ�־λ
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//�ж����ã�ѡ������ж�,ʹ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//��ʱ��ͨ�����ã�ѡ��TIM2_IRQn
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructure);//��ʱ����ʼ��
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//��ȡ�жϱ�־λ����������ʱ��2�������±�־λ
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����־λ
	}
}
*/