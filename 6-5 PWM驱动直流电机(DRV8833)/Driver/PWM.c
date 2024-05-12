#include "stm32f10x.h"                  // Device header

void PWM1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	
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
	TIM_TimeBaseInitStructure.TIM_Prescaler= 36-1;//PSR
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
	TIM_OC3Init(TIM2,&TIM_OCInit_Structure);//ͨ��3��ʼ��
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

void PWM2_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//����TIM2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ӳ��*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ������ȿ���AFIO��ʱ��
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);			//��TIM2�����Ų�����ӳ�䣬�����ӳ�䷽����鿴�ο��ֲ�
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//��JTAG����ʧ�ܣ���Ϊ��ͨGPIO����ʹ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��PA0���ų�ʼ��Ϊ�����������	
																	//��������Ƶ����ţ�����Ҫ����Ϊ����ģʽ		
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM2);		//ѡ��TIM2Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;					//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
	/*����Ƚϳ�ʼ��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����ṹ�����
	TIM_OCStructInit(&TIM_OCInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//����Ƚ�ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������ԣ�ѡ��Ϊ�ߣ���ѡ����Ϊ�ͣ�������ߵ͵�ƽȡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//��ʼ��CCRֵ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);						//���ṹ���������TIM_OC1Init������TIM2������Ƚ�ͨ��1
	
	/*TIMʹ��*/
	TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
}

void PWM_Init(void)
{
	PWM1_Init();
	PWM2_Init();
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}
//���й��̸���CCRֵ
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
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