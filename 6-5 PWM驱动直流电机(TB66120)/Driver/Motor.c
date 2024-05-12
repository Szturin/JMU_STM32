#include "stm32f10x.h"                  // Device header
#include <PWM.h>

/*宏定义，方便替换引脚*/
#define	LOGIC1 GPIO_Pin_5
#define	LOGIC2 GPIO_Pin_4

/**
  * @函	数		直流电机初始化
  * @参	数		无
  * @返回值		无
  * @特殊说明		LOGIC1和LOGIC为控制电机驱动模块的逻辑端口,PWM为TB66120的PWM输入端	
  */
void Motor_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; //结构体类型(已经定义好的） 结构体变量名 ->结构体变量的定义
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=LOGIC1|LOGIC2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LOGIC1|LOGIC2);//初始化默认低电平输出，所以要置高电平
	
	PWM_Init();
}

/**
  * @函	数		直流电机速度设置
  * @参	数		速度
  * @返回值		无
  * @特殊说明		这里的速度特指PWM占空比，理论最大值为100，为负数时控制电机反转
  */
void Motor_SpeedSet(int8_t Speed)
{
	if(Speed>0)
	{
		GPIO_SetBits(GPIOA, LOGIC1);
		GPIO_ResetBits(GPIOA, LOGIC2);	
		PWM_SetCompare3(Speed);				
	}
	else
	{
		GPIO_SetBits(GPIOA, LOGIC2);	
		GPIO_ResetBits(GPIOA, LOGIC1);	
		PWM_SetCompare3(-Speed);//输入参数为负数时，自取反
	}
}