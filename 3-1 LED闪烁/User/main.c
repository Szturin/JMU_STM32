#include <stm32f10x.h> 			//Device header
#include <Delay.h>

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//一般输出用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0  ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//四个基本的输出电平设置函数
	//GPIO_SetBits(GPIOA,GPIO_Pin_0);//LED灭
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0);//LED亮
	//GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);//LED亮
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);//LED灭
	
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay_ms(100);
	}
}
