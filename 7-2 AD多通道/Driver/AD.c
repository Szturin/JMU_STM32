#include "stm32f10x.h"                  // Device header

//��ͨ��
void AD_Init()
{
	/*RCCʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC����APB2�ϵ��豸
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//���Ƶ��14MHZ,����ѡ��6��Ƶ��12MHZ
	
	/*GPIO����*/
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ������(�Ѿ�����õģ� �ṹ������� ->�ṹ������Ķ���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������ģʽ���Ͽ�GPIO����ֹGPIO�����������ģ���ѹ��ɸ���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ADC��ؼĴ�������*/
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//�ڹ�����˵��б��һ��λ�ã�д��ͨ��0
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//����ģʽ
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//��ʹ���ⲿ����
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=1;//����ɨ��ģʽ�²���Ҫ����1
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//��ɨ��ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);//����ADC��Դ
	
	ADC_ResetCalibration(ADC1);//��λУ׼,����CR2_RSTCAL_Set
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//�Ƿ�λ���
	ADC_StartCalibration(ADC1);//����У׼
	while(ADC_GetCalibrationStatus(ADC1) == SET);//У׼��־λ�жϣ��ȴ�У׼���
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);//�ڹ�����˵��б��һ��λ�ã�д��ͨ��
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������ת��
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//�ȴ�ת����ɱ�־λ��1
	return ADC_GetConversionValue(ADC1);
}