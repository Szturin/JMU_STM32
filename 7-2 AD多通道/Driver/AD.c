#include "stm32f10x.h"                  // Device header

//多通道
void AD_Init()
{
	/*RCC时钟配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC属于APB2上的设备
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//最大频率14MHZ,所以选择6分频，12MHZ
	
	/*GPIO配置*/
	GPIO_InitTypeDef GPIO_InitStructure; //结构体类型(已经定义好的） 结构体变量名 ->结构体变量的定义
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入模式，断开GPIO，防止GPIO的输入输出对模拟电压造成干扰
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ADC相关寄存器配置*/
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//在规则组菜单列表第一个位置，写入通道0
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//不使用外部触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_NbrOfChannel=1;//仅在扫描模式下才需要大于1
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//非扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);//开启ADC电源
	
	ADC_ResetCalibration(ADC1);//复位校准,操作CR2_RSTCAL_Set
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//是否复位完成
	ADC_StartCalibration(ADC1);//启动校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);//校准标志位判断，等待校准完成
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);//在规则组菜单列表第一个位置，写入通道
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//等待转换完成标志位置1
	return ADC_GetConversionValue(ADC1);
}