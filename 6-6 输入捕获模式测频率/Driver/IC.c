#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM2时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; //结构体类型(已经定义好的） 结构体变量名 ->结构体变量的定义
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;	
	//为什么配置成开漏推挽输出？
	//：对于普通的推挽输出，引脚的输出控制是来自与数据寄存器的
	//复用开漏推挽输出，将输出控制权交给片上外设，PWM波形才能由定时器控制
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*时基单元初始化配置*/
	TIM_InternalClockConfig(TIM3);//定时器上电后默认使用内部时钟,此语句也可以不写
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Prescaler= 72-1;//PSC
	TIM_TimeBaseInitStructure.TIM_Period= 65536 - 1;//ARR 这里测量PWM的频率，需要较大的计数空间
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器，高级定时器钟存在，这里不需要用
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//时基单元初始化
	
	/*输入捕获单元配置*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//输入捕获通道选择
	TIM_ICInitStructure.TIM_ICFilter=0XF;//输入捕获滤波器
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//极性，上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//分频值
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//选择从哪个引脚输入，直连通道输入
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	/*从模式配置*/
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//触发源选择
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);//配置从模式为RESET
	
	TIM_Cmd(TIM3, ENABLE);			//使能TIM2，定时器开始运行
}

//获取PWM频率 
uint32_t IC_GetFreq(void)
{
	return 1000000/ (TIM_GetCapture1(TIM3)+1);//每次计数误差会多1，所以这里分母+1
}