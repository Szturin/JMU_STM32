#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����TIM2ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;	
	//Ϊʲô���óɿ�©���������
	//��������ͨ��������������ŵ�������������������ݼĴ�����
	//���ÿ�©������������������Ȩ����Ƭ�����裬PWM���β����ɶ�ʱ������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ʱ����Ԫ��ʼ������*/
	TIM_InternalClockConfig(TIM3);//��ʱ���ϵ��Ĭ��ʹ���ڲ�ʱ��,�����Ҳ���Բ�д
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Prescaler= 72-1;//PSC
	TIM_TimeBaseInitStructure.TIM_Period= 65536 - 1;//ARR �������PWM��Ƶ�ʣ���Ҫ�ϴ�ļ����ռ�
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ���Ӵ��ڣ����ﲻ��Ҫ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	
	/*���벶��Ԫ����*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//���벶��ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICFilter=0XF;//���벶���˲���
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//���ԣ������ش���
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//��Ƶֵ
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ѡ����ĸ��������룬ֱ��ͨ������
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	/*��ģʽ����*/
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//����Դѡ��
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);//���ô�ģʽΪRESET
	
	TIM_Cmd(TIM3, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
}

//��ȡPWMƵ�� 
uint32_t IC_GetFreq(void)
{
	return 1000000/ (TIM_GetCapture1(TIM3)+1);//ÿ�μ��������1�����������ĸ+1
}