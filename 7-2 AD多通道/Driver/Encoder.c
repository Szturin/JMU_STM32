#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����TIM2ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ʱ����Ԫ��ʼ������*/
	TIM_InternalClockConfig(TIM3);//��ʱ���ϵ��Ĭ��ʹ���ڲ�ʱ��,�����Ҳ���Բ�д
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Prescaler= 1-1;//PSC ����Ƶ
	TIM_TimeBaseInitStructure.TIM_Period= 65536 - 1;//ARR 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�ظ����������߼���ʱ���Ӵ��ڣ����ﲻ��Ҫ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//ʱ����Ԫ��ʼ��
	
	/*���벶��Ԫ����*/
	/**ͨ��1����**/
	TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ�帳��ʼֵ����ֹ���ֲ�ȷ����״̬
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//���벶��ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICFilter=0XF;//���벶���˲���
	//TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//���ԣ������ش���  TIM_EncoderInterfaceConfig�ظ���������ͬ�ļĴ���
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
    /**ͨ��2����**/ //ע�⣺���ﲻ��Ҫ�ٶ����µĽṹ�塣��Ϊ��һ�ε�ֵ�Ѿ�д��Ӳ���Ĵ���
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//���벶��ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICFilter=0XF;//���벶���˲���
	//TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//���ԣ������ش���
	TIM_ICInit(TIM3,&TIM_ICInitStructure);

    /*���ñ������ӿ�*/
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//��ʱ��3 ��T1T2ͬʱʹ�ã�������ͬ(������/�½�������������Ч��)����Ϊ����
    //��Ҫ���࣬��ôһ������λ�����أ�һ������Ϊ�½�����Ч
	TIM_Cmd(TIM3,ENABLE);//������ʱ��
}

int16_t Encoder_GetCounter(void)//int16_t ��ʾ�з��������ܰ�unint16_������λ��Ϊ�˷���λ,�������õ��˲��������
{
	return TIM_GetCounter(TIM3);
}

int16_t Encoder_GetSpeed(void)//int16_t ��ʾ�з��������ܰ�unint16_������λ��Ϊ�˷���λ,�������õ��˲��������
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}