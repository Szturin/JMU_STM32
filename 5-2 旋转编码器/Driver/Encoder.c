#include "stm32f10x.h"                  // Device header
int16_t Encoder_Count;
void Encoder_Init(void)
{
	/*�ⲿ�жϵĶ���*/
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����AFIO��ʱ�ӣ��ⲿ�жϱ��뿪��AFIO��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PB14���ų�ʼ��Ϊ��������
	
	/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//���ⲿ�жϵ�14����ӳ�䵽GPIOB����ѡ��PB14Ϊ�ⲿ�ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1;					//ѡ�������ⲿ�жϵ�14����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//ָ���ⲿ�ж���Ϊ�½��ش���
	EXTI_Init(&EXTI_InitStructure);								//���ṹ���������EXTI_Init������EXTI����
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
																//�˷������������������н������һ��
																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		//ѡ������NVIC��EXTI0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		//ѡ������NVIC��EXTI1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����	
}

int Encoder_Get_value(void)//ֱ�ӵõ�����ֵ
{
	return Encoder_Count;
}

int Encoder_Get_Change_value(void)//ֱ�ӵõ�����ֵ
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���Encoder_Count��������*/
	/*�����Ҳ����ֱ�ӷ���Encoder_Count
	  �������Ͳ��ǻ�ȡ����ֵ�Ĳ���������
	  Ҳ����ʵ�ֹ��ܣ�ֻ��˼·��һ��*/
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
			{
				Encoder_Count--;
			}	
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}	

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
			{
				Encoder_Count++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}	
