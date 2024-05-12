#include "stm32f10x.h"

/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_All);//��ʼ��Ĭ�ϵ͵�ƽ���������Ҫ�øߵ�ƽ
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}
/**
  * @brief 
  * @param  
  * @retval 		
  */
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}
