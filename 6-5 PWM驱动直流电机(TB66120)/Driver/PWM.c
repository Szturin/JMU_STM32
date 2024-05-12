#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM2时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure; //结构体类型(已经定义好的） 结构体变量名 ->结构体变量的定义
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	
	//为什么配置成开漏推挽输出？
	//：对于普通的推挽输出，引脚的输出控制是来自与数据寄存器的
	//复用开漏推挽输出，将输出控制权交给片上外设，PWM波形才能由定时器控制
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用开漏推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*时基单元初始化配置*/
	TIM_InternalClockConfig(TIM2);//定时器上电后默认使用内部时钟,此语句也可以不写
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	/*时基单元关键寄存器*/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 72 - 1;//PSC 舵机要求的频率是50HZ
	TIM_TimeBaseInitStructure.TIM_Period= 20000 - 1;//ARR
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器钟存在，这里不需要用
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//时基单元初始化
	
	/*TIM2通道1通道*/
	//TIM2的ETR引脚和通道1的引脚都是借用了PA0的引脚
	//TIM2的OC1，输出PWM，只能在PA0的引脚,也可以使用重映射，使用其他能够映射的引脚
	TIM_OCInitTypeDef TIM_OCInit_Structure;
	TIM_OCStructInit(&TIM_OCInit_Structure);//给结构体赋初始值
	TIM_OCInit_Structure.TIM_OCMode=TIM_OCMode_PWM1;//PWM模式1
	TIM_OCInit_Structure.TIM_OCPolarity=TIM_OCNPolarity_High;//高极性
	TIM_OCInit_Structure.TIM_OutputState=TIM_OutputState_Enable;//输出使能
	TIM_OCInit_Structure.TIM_Pulse=0;//设置CCR
	TIM_OC2Init(TIM2,&TIM_OCInit_Structure);//通道2初始化
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

//运行过程更改CCR值
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}
/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//获取中断标志位，参数：定时器2，检测更新标志位
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
	}
}
*/