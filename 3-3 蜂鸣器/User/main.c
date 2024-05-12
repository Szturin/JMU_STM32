#include <stm32f10x.h> 			//Device header
#include <Delay.h>


unsigned char i;
unsigned char temp;
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//一般输出用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//可以用按位或的方式同时初始化多个引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	while(1)
	{
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(700);	
	}
}
