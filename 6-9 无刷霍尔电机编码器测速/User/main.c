#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <OLED.h>
#include <Encoder.h>
#include <Timer.h>
#include <Motor.h>
#include <Key.h>

//定时器定时速度过慢？？？
//此工程运行正常

int8_t PWM;
int16_t Speed;
unsigned int number;
unsigned char KeyNum;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"Speed:");
	Key_Init();//KEY没有初始化，导致Angle一直自动加
	Motor_PWM_Init();
	OLED_ShowString(3,1,"PWM:");
	while(1)
	{
		OLED_ShowSignedNum(2,7,number,5);
		OLED_ShowSignedNum(1,7,Speed,5);
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			PWM=PWM+20;
			if(PWM==120){PWM=0;}
		}
		else if(KeyNum==2)
		{
			PWM=PWM-20;
			if(PWM==-120){PWM=0;}
		}
		OLED_ShowSignedNum(3,7,PWM,4);
		Motor_SpeedSet(PWM);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//获取中断标志位，参数：定时器2，检测更新标志位
	{
		Speed=Encoder_GetSpeed();
		number++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
	}
}
