#include <stm32f10x.h> 			//Device header

int main(void)
{
	/*ʹ�����üĴ����ķ�ʽ���е�Ʋ���*/
//	RCC->APB2ENR = 0x00000010;
//	GPIOC->CRH = 0x00300000;
//	GPIOC->ODR = 0x00002000; 
	//ȱ�㣺���÷�����������
	//�������ܣ�ʹ��/ʧ��APB2������ʱ��
	
	/*ʹ�ÿ⺯���ķ�ʽ���е�Ʋ���*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//����GPIOC������ʱ��
	//�����ı��ʻ������üĴ��������Ƿ�װ���������ɿ��Ĺ���
	//����Ӱ�쵽�Ĵ���������λ������Ҫ�ֶ�����Ĵ���
	GPIO_InitTypeDef GPIO_InitSructure;//�ṹ���������
	GPIO_InitSructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitSructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitSructure);
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);//����PC13��Ϊ�ߵ�ƽ
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//����PC13��Ϊ�͵�ƽ
	while(1)
	{
		
	}
}
