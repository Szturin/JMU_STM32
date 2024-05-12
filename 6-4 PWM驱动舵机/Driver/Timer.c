#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM2时钟
	
	/*时基单元初始化配置*/
	TIM_InternalClockConfig(TIM2);//定时器上电后默认使用内部时钟,此语句也可以不写
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	/*时基单元关键寄存器*/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 7200 - 1;//ARR ， 对72MHZ进行7200分频 也就是10MHZ
	TIM_TimeBaseInitStructure.TIM_Period= 10000 - 1;//PSC，计数器重装载值，10000次清零,控制ARR和PSC可以控制计数频率
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器钟存在，这里不需要用
	
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//时基单元初始化
	//1-->注意：更新中断和更新事件同时发生，初始化会立刻进入中断
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//2-->手动清理中断标志位
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//中断配置，选择更新中断,使能
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;//定时器通道设置，选择TIM2_IRQn
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructure);//定时器初始化
	
	TIM_Cmd(TIM2,ENABLE);//启动定时器
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