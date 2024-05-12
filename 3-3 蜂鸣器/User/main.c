#include <stm32f10x.h> 			//Device header
#include <Delay.h>


unsigned char i;
unsigned char temp;
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//һ��������������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//�����ð�λ��ķ�ʽͬʱ��ʼ���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	while(1)
	{
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(50);		
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			Delay_ms(700);	
	}
}
