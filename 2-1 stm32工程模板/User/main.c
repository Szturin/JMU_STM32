#include <stm32f10x.h> 			//Device header

int main(void)
{
	/*使用配置寄存器的方式进行点灯操作*/
//	RCC->APB2ENR = 0x00000010;
//	GPIOC->CRH = 0x00300000;
//	GPIOC->ODR = 0x00002000; 
	//缺点：配置繁琐，操作多
	//函数功能：使能/失能APB2的外设时钟
	
	/*使用库函数的方式进行点灯操作*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//配置GPIOC的外设时钟
	//函数的本质还是配置寄存器，但是封装好了完整可靠的功能
	//不会影响到寄存器的其他位，不需要手动计算寄存器
	GPIO_InitTypeDef GPIO_InitSructure;//结构体变量定义
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitSructure);
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);//设置PC13口为高电平
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//设置PC13口为低电平
	while(1)
	{
		
	}
}
