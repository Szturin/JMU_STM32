#include "sys.h"
#include "usart.h"	
/////////////////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
///////////////////////////////////////////////////////////////////////////////
char bluetooth=90;
extern int velocity_calcu; //速度理论值	
extern float yaw_calcu;    //yaw角理论值



void usart2_init(u32 bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			          //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                  //初始化GPIOA.2
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;               //PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                  //初始化GPIOA.3 

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = bound;                     //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;             //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure);      //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口2接受中断
  USART_Cmd(USART2, ENABLE);                     //使能串口2

}

//串口2中断服务程序：
void USART2_IRQHandler(void)          	
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		bluetooth = USART_ReceiveData(USART2); //通过串口2接收数据   	
		
		//【1.调转向】
		if(bluetooth==71) 
		{
			yaw_calcu=yaw_calcu-10;
			if(yaw_calcu<-160) yaw_calcu=-160;
		}
		if(bluetooth==67) 
		{
			yaw_calcu=yaw_calcu+10;
			if(yaw_calcu>160) yaw_calcu=160;
		}
		
		//【2.调前后跑】
		if(bluetooth==65) velocity_calcu=20;
		if(bluetooth==69) velocity_calcu=-20;
		if(bluetooth==90) velocity_calcu=0;
  } 
} 


