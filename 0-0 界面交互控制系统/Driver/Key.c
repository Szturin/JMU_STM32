#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIOB�ⲿʱ������
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�������Ƶ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������ģʽ
	GPIO_Init(GPIOB,&GPIO_InitStructure);//���ų�ʼ��
}

uint8_t Key_GetNum(void) //uint8_t�൱��unsigned char 
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);
		KeyNum= 2;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(20);
		KeyNum= 1;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0);
		Delay_ms(20);
		KeyNum= 3;
	}
	return KeyNum;
}