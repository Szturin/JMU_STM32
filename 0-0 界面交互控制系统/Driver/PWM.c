#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��AFIOʱ��
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//PA0ӳ�䵽PA15
//	//PA15Ĭ��Ϊ���Զ˿ڵĸ���
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�����ӳ��˿�
	
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	
	//Ϊʲô���óɿ�©���������
	//��������ͨ��������������ŵ�������������������ݼĴ�����
	//���ÿ�©������������������Ȩ����Ƭ�����裬PWM���β����ɶ�ʱ������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//���ÿ�©�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ʱ����Ԫ��ʼ������*/
	TIM_InternalClockConfig(TIM2);//��ʱ���ϵ��Ĭ��ʹ���ڲ�ʱ��,�����Ҳ���Բ�д
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	/*ʱ����Ԫ�ؼ��Ĵ���*/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 720 - 1;//ARR �� ��72MHZ����7200��Ƶ Ҳ����10MHZ
	TIM_TimeBaseInitStructure.TIM_Period= 100 - 1;//PSC����������װ��ֵ��10000������,����ARR��PSC���Կ��Ƽ���Ƶ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ���Ӵ��ڣ����ﲻ��Ҫ��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	
	/*TIM2ͨ��1ͨ��*/
	//TIM2��ETR���ź�ͨ��1�����Ŷ��ǽ�����PA0������
	//TIM2��OC1�����PWM��ֻ����PA0������,Ҳ����ʹ����ӳ�䣬ʹ�������ܹ�ӳ�������
	TIM_OCInitTypeDef TIM_OCInit_Structure;
	TIM_OCStructInit(&TIM_OCInit_Structure);//���ṹ�帳��ʼֵ
	TIM_OCInit_Structure.TIM_OCMode=TIM_OCMode_PWM1;//PWMģʽ1
	TIM_OCInit_Structure.TIM_OCPolarity=TIM_OCNPolarity_High;//�߼���
	TIM_OCInit_Structure.TIM_OutputState=TIM_OutputState_Enable;//���ʹ��
	//Ƶ��Ϊ1KHZ,ռ�ձ�Ϊ50%
	TIM_OCInit_Structure.TIM_Pulse=50;//����CCR
	TIM_OC1Init(TIM2,&TIM_OCInit_Structure);
	
	/*����Ҫ�жϣ�����ΪӲ��PWM*/
//	//1-->ע�⣺�����жϺ͸����¼�ͬʱ��������ʼ�������̽����ж�
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//2-->�ֶ������жϱ�־λ
//	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//�ж����ã�ѡ������ж�,ʹ��
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//��ʱ��ͨ�����ã�ѡ��TIM2_IRQn
//	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	
//	NVIC_Init(&NVIC_InitStructure);//��ʱ����ʼ��
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

//���й��̸���CCRֵ
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
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