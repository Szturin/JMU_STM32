#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�����ӳ��˿�
	
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
	TIM_TimeBaseInitStructure.TIM_Prescaler= 720 - 1;//PSC
	TIM_TimeBaseInitStructure.TIM_Period= 100 - 1;//ARR
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
	TIM_OCInit_Structure.TIM_Pulse=0;//����CCR
	TIM_OC1Init(TIM2,&TIM_OCInit_Structure);
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

void PWM_ON(void)
{
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_OFF(void)
{
	TIM_Cmd(TIM2,DISABLE);
}

//���й��̸���CCRֵ
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

//���й��̸���PSR��ֵ
void PWM_SetPrescaler(uint16_t PSC)
{
	TIM_PrescalerConfig(TIM2,PSC,TIM_PSCReloadMode_Immediate);
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