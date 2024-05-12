/******************** (C) COPYRIGHT (2015)BST BALANCECAR **************************
 * 文件名  ：main.c
**********************************************************************************/


#include "motor.h"

#include "timer.h"



/*协议相关*/
//extern u8 newLineReceived = 0;

/*
 * 函数名：main
 * 描述  ：主函数
 */
 
 void delay(unsigned int bx)
 {

   unsigned int x,y;
    for(x=bx;x>0;x--)
        for(y=110;y>0;y--);

}
int main(void)
{	
       
	TIM2_PWM_Init();					   //PWM输出初始化
	MOTOR_GPIO_Config();				  //电机IO口初始化
	Timerx_Init(50,7199);				   //定时器TIM1

	while (1)
	{

	}
 								    
}
