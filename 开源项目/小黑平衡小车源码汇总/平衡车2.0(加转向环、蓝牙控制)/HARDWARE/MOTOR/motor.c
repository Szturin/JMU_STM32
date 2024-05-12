#include "motor.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
void SETPWM(int PWM1,int PWM2)
{
	if(PWM1>0) //正转
	{
		left_positive();
		TIM_SetCompare1(TIM1,7200-PWM1);    
	}
	else       //反转
	{
		left_negative();
		TIM_SetCompare1(TIM1,7200-(-PWM1) );       
	}	
	
	if(PWM2>0) //正转
	{
		right_positive();
		TIM_SetCompare4(TIM1,PWM2);     
	}
	else       //反转
	{
		right_negative();
		TIM_SetCompare4(TIM1,-PWM2); 
	}
}

//void SETPWM(int PWM1,int PWM2)
//{
//	if(PWM1>0) //正转
//	{
//		left_positive();
//		//right_positive();
//		TIM_SetCompare1(TIM1,7200-PWM1);
//		//TIM_SetCompare4(TIM1,PWM1);     
//	}
//	else      //反转
//	{
//		left_negative();
//		//right_negative();
//		TIM_SetCompare1(TIM1,7200-(-PWM1) ); 
//		//TIM_SetCompare4(TIM1,-PWM1 );      
//	}	
//}


void PWM_Xianfu(int max,int *PWM1,int *PWM2)
{
	if(*PWM1>max)  *PWM1 = max;
	if(*PWM1<-max) *PWM1 =-max;
	
	if(*PWM2>max)  *PWM2 = max;
	if(*PWM2<-max) *PWM2 =-max;
}


void left_positive(void) //左轮正传
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void left_negative(void) //左轮反转
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void right_positive(void) //右轮正转
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void right_negative(void) //右轮反转
{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}


void motor_init()
{
	motor_gpio_init();         //输出pwm的gpio和控制转向的gpio初始化
	TIM1_PWM_Init(7200-1,1-1); //TIM1的pwm模式初始化
	TIM3_encoder_init();       //TIM3的编码器模式初始化
	TIM2_encoder_init();       //TIM2的编码器模式初始化
}


//PWM：   左PA8
//        右PA11
//正反转：左PB14、15
//        右PB13、12
void motor_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//PWM口初始化：
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1  TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//正反转控制口初始化：
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12; //右正反转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15; //左正反转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


int read_encoder3(void)
{
	int speed;
	speed = (short)TIM_GetCounter(TIM3); //1.采集编码器的计数值并保存
	TIM_SetCounter(TIM3,0);              //2.将定时器的计数值清零
	
	return speed;
}

int read_encoder2(void)
{
	int speed;
	speed = (short)TIM_GetCounter(TIM2); //1.采集编码器的计数值并保存
	TIM_SetCounter(TIM2,0);              //2.将定时器的计数值清零
	
	return speed;
}

//void motor_enable(float pitch) //根据角度判断是否使能 or 关闭电机
//{
//	if(pitch>15 || pitch<-15)   
//	{
//		motor_flag=0;     //表示要关闭电机
//		velocity_sum = 0; //速度积分清0
//	}
//	if(pitch>=-15 && pitch<=15) motor_flag=1; //表示要开启电机
//	
//	
//	
//	if(motor_flag==1 && picture_flag1==1) 
//	{
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1); //显示立着的图片
//		
//		picture_flag1 = -1;
//	}
//	
//	if(motor_flag==0 && picture_flag1==-1) 
//	{
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP0); //显示倒了的图片
//		
//		picture_flag1 = 1;
//	}
//}

