#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM2时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开AFIO时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//PA0映射到PA15
//	//PA15默认为调试端口的复用
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//解除重映射端口
	
	GPIO_InitTypeDef GPIO_InitStructure; //结构体类型(已经定义好的） 结构体变量名 ->结构体变量的定义
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	
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
	TIM_TimeBaseInitStructure.TIM_Prescaler= 720 - 1;//ARR ， 对72MHZ进行7200分频 也就是10MHZ
	TIM_TimeBaseInitStructure.TIM_Period= 100 - 1;//PSC，计数器重装载值，10000次清零,控制ARR和PSC可以控制计数频率
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
	//频率为1KHZ,占空比为50%
	TIM_OCInit_Structure.TIM_Pulse=50;//设置CCR
	TIM_OC1Init(TIM2,&TIM_OCInit_Structure);
	
	/*不需要中断，这里为硬件PWM*/
//	//1-->注意：更新中断和更新事件同时发生，初始化会立刻进入中断
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//2-->手动清理中断标志位
//	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//中断配置，选择更新中断,使能
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//定时器通道设置，选择TIM2_IRQn
//	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	
//	NVIC_Init(&NVIC_InitStructure);//定时器初始化
	
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

//运行过程更改CCR值
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
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