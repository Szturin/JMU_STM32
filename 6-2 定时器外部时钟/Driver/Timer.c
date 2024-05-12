#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIOAʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//TIM_InternalClockConfig(TIM2);//��ʱ���ϵ��Ĭ��ʹ���ڲ�ʱ��,�����Ҳ���Բ�д
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);//ͨ��ETR���ŵ��ⲿʱ��ģʽ2����
	//��ʱ��ѡ��    ����Ҫ��Ƶ    �ⲿ�����ļ��ԣ�������     �ⲿ�����˲���,����F��N�����ﲻ��Ҫ�˲���
	
	/*ʱ����Ԫ��ʼ������*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	/**ʱ����Ԫ�ؼ��Ĵ���**/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 1 - 1;//ARR �� ��72MHZ����7200��Ƶ Ҳ����10MHZ
	TIM_TimeBaseInitStructure.TIM_Period= 10 - 1;//PSC����������װ��ֵ��10000������,����ARR��PSC���Կ��Ƽ���Ƶ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ���Ӵ��ڣ����ﲻ��Ҫ��
	
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	//1-->ע�⣺�����жϺ͸����¼�ͬʱ��������ʼ�������̽����ж�
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//2-->�ֶ������жϱ�־λ
	
	/*�ж�����*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//�ж����ã�ѡ������ж�,ʹ��
	
	
	/*NVIC����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//��ʱ��ͨ�����ã�ѡ��TIM2_IRQn
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

//CNTֵ��װ����
uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
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