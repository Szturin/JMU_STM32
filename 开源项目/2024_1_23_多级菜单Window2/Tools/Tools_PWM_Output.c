#include "Tools.h"

struct Option_Class PWM_Output_Option_List[];
void Disable_MUReadKey(void);

struct Tools_PWM_Output_Class
{
	uint8_t isInit;
	float PSC;
	float CCR;
	float ARR;
	float Freq;
	float Duty; // percent*100
	struct Menu_Class MU;
} PWM_Output = {
	.isInit = 1,
	.PSC = 719,
	.CCR = 50,
	.ARR = 99,
	.Freq = 1000,
	.Duty = 50,
	.MU = {
		.isInit = 1,
		.isShow = 1,
		.isReadKey = 1,
		.Cat_i = 1,
		.Font_Width = 6,
		.Font_Height = 8,
		.Line_Height = 12,
		.Cur_Style = reverse,
		.Layout_Margin = 2, 
		.Option_MaxNum = 6,
		.Target_Window_X = 0,
		.Target_Window_Y = 0,
		.Target_Window_W = 80,
		.Target_Window_H = 64,
		.Option_List = PWM_Output_Option_List},
};
struct Option_Class PWM_Output_Option_List[] = {
	{"<<<"},
	{"Freq:%.1f", Disable_MUReadKey, &PWM_Output.Freq, 0, 0, 80, 13},
	{"Duty:%.1f", Disable_MUReadKey, &PWM_Output.Duty, 0, 12, 80, 13},
	{"PSC: %.1f", Disable_MUReadKey, &PWM_Output.PSC, 0, 24, 80, 13},
	{"CCR: %.1f", Disable_MUReadKey, &PWM_Output.CCR, 0, 36, 80, 13},
	{"ARR: %.1f", Disable_MUReadKey, &PWM_Output.ARR, 0, 48, 80, 13},
	{".."},
};

void Disable_MUReadKey(void)
{
	PWM_Output.MU.Cur_Style = mouse;
	PWM_Output.MU.isReadKey = 0;
}

void Tools_PWM_Output_SetFreq(float SetFreq) // 设置输出频率
{
	if (SetFreq > 10000)
	{
		SetFreq = 10000;
	}
	if (SetFreq < 1)
	{
		SetFreq = 1;
	}
	PWM_Output.PSC = ((float)72000000 / (PWM_Output.ARR + 1)) / (SetFreq)-1;
	if (PWM_Output.PSC < 0)
	{
		PWM_Output.PSC = 0;
	}
	PWM_Output.Freq = ((float)72000000 / (PWM_Output.ARR + 1)) / (PWM_Output.PSC + 1);
	PWM_SetPrescaler(PWM_Output.PSC);
}

void Tools_PWM_Output_SetDuty(float SetDuty) // 设置输出空占比
{
	if (SetDuty < 0)
	{
		SetDuty = 0;
	}
	if (SetDuty > 100)
	{
		SetDuty = 100;
	}
	PWM_Output.CCR = (SetDuty / 100) * (PWM_Output.ARR + 1);
	PWM_Output.Duty = (float)PWM_Output.CCR / (PWM_Output.ARR + 1) * 100;
	PWM_SetCompare1(PWM_Output.CCR);
}

void Tools_PWM_Output_SetPSC(float PSC) //
{
	PWM_Output.PSC = PSC;
	PWM_Output.Freq = ((float)72000000 / (PWM_Output.ARR + 1)) / (PWM_Output.PSC + 1);
	PWM_SetPrescaler(PWM_Output.PSC);
}
void Tools_PWM_Output_SetCCR(float CCR) //
{
	PWM_Output.CCR = CCR;
	PWM_Output.Duty = (float)PWM_Output.CCR / (PWM_Output.ARR + 1) * 100;
	PWM_SetCompare1(PWM_Output.CCR);
}
void Tools_PWM_Output_SetARR(float ARR) //
{
	PWM_Output.ARR = ARR;
	PWM_Output.Freq = ((float)72000000 / (PWM_Output.ARR + 1)) / (PWM_Output.PSC + 1);
	PWM_Output.Duty = (float)PWM_Output.CCR / (PWM_Output.ARR + 1) * 100;
	PWM_SetPeriod(PWM_Output.ARR);
}

void Tools_PWM_OutputUpdate(void) // 更新输出频率及占空比数值
{
	PWM_Output.Freq = ((float)72000000 / (PWM_Output.ARR + 1)) / (PWM_Output.PSC + 1);
	PWM_Output.Duty = (float)PWM_Output.CCR / (PWM_Output.ARR + 1) * 100;
	PWM_SetCompare1(PWM_Output.CCR);
	PWM_SetPrescaler(PWM_Output.PSC);
	PWM_SetPeriod(PWM_Output.ARR);
}

void Tools_PWM_Output(void)
{
	static float NumPow = 1;

	//PWM_Output.MU.isCaller = 0;
	
	if (PWM_Output.isInit)
	{
		//Menu_MenuClassInit(&PWM_Output.MU, PWM_Output_Option_List);
		
		PWM_Output.isInit = 0;
		PWM_Init(); // PWM初始化
		IC_Init();	// 输入捕获初始化

		PWM_Output.PSC = 71;
		PWM_Output.CCR = 2500;
		PWM_Output.ARR = 19999;
		Tools_PWM_OutputUpdate();
	}

	while (1)
	{
		OLED_Clear();

		if (PWM_Output.MU.isReadKey == 0) // 如果菜单不读取按键则代表抓住了选项;
		{
			switch (PWM_Output.MU.Cat_i) // 抓住的选项
			{
			case 1:
				PWM_Output.Freq += Menu_RollEvent() * NumPow; // 频率加上 编码器偏移量乘以位权
				Tools_PWM_Output_SetFreq(PWM_Output.Freq);	  // 设置频率
				break;
			case 2:
				PWM_Output.Duty += Menu_RollEvent() * NumPow / 10;
				Tools_PWM_Output_SetDuty(PWM_Output.Duty); // 设置频率
				break;
			case 3:
				PWM_Output.PSC += Menu_RollEvent() * NumPow;
				Tools_PWM_Output_SetPSC(PWM_Output.PSC);
				break;
			case 4:
				PWM_Output.CCR += Menu_RollEvent() * NumPow;
				Tools_PWM_Output_SetCCR(PWM_Output.CCR);
				break;
			case 5:
				PWM_Output.ARR += Menu_RollEvent() * NumPow;
				Tools_PWM_Output_SetARR(PWM_Output.ARR);
				break;
			default:
				break;
			}
			if (Menu_EnterEvent())// 按确定键放手
			{
				Tools_PWM_Output_SetPSC((uint16_t)PWM_Output.PSC);
				Tools_PWM_Output_SetCCR((uint16_t)PWM_Output.CCR);
				NumPow = 1;
				PWM_Output.MU.isReadKey = 1; 
			}
			if (Menu_BackEvent()) // 按返回键加位权
			{
				NumPow *= 10; // 位权
				if (NumPow > 100)
				{
					NumPow = 1;
				}
			}
		}
		else
		{
			PWM_Output.MU.Cur_Style = Menu_Global.Cur_Style;
		}

		OLED_Printf(82, 10, 6, "ICFreq\n:%d\n\nICDuty\n:%d%%", IC_GetFreq(), IC_GetDuty());	// 不断刷新显示输入捕获测得的频率// 不断刷新显示输入捕获测得的占空比

		if (Menu_RunWindow(&PWM_Output.MU) == 0) // 如果不显示选项
		{
			PWM_Output.isInit = 1;
			PWM_Output.MU.Window_W = 0;
			PWM_Output.MU.Window_H = 0;
			
			return;
		}

		OLED_Update();
	}
}
