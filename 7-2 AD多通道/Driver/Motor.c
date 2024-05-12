#include "stm32f10x.h"                  // Device header
#include <PWM.h>

/*�궨�壬�����滻����*/
#define	LOGIC1 GPIO_Pin_8
#define	LOGIC2 GPIO_Pin_9

void Motor_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=LOGIC1|LOGIC2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LOGIC1|LOGIC2);//��ʼ��Ĭ�ϵ͵�ƽ���������Ҫ�øߵ�ƽ
	
	PWM_Init();
}

void Motor_SpeedSet(int8_t Speed)
{
	if(Speed>0)
	{
		PWM_SetCompare3(Speed);PWM_SetCompare1(0);
	}
	else
	{
		PWM_SetCompare1(-Speed);PWM_SetCompare3(0);	
	}
}