#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include "stdio.h"

unsigned char i;
unsigned char temp;
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//一般输出用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;//可以用按位或的方式同时初始化多个引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while(1)
	{
		for(i=0;i<8;i++)
		{
			GPIO_Write(GPIOA,~(0x0001<<i)); //0000 0000 0000 0001
			Delay_ms(20);		
		}
	}
}
