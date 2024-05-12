#include "Menu.h"

void Menu_RunMainMenu(void)
{
	static struct Option_Class Menu_StartOptionList[] = {
		{"<<<"},
		{"Tools", Menu_RunToolsMenu},	   // 工具
		{"Games", Menu_RunGamesMenu},	   // 游戏
		{"Setting", Menu_RunSettingMenu},  // 设置
		{"Information", Menu_Information}, // 信息
		{".."}};

	Menu_RunMenu(Menu_StartOptionList);
}

void Menu_RunToolsMenu(void)
{
	static struct Option_Class Menu_ToolsOptionList[] = {
		{"<<<"},
		{"定时器"},					   // 6-1 定时器定时中断
		{"输入捕获"},				   // 6-6 输入捕获模式测频率
		{"PWM输出", Tools_PWM_Output}, // 6-3 PWM驱动LED呼吸灯
		{"PWM输入"},				   // 6-7 PWMI模式测频率占空比
		{"编码器"},					   // 6-8 编码器接口测速
		{"串口"},					   // 9-3 串口收发HEX数据包
		{"示波器"},					   // 示波器
		{"ADC"},					   // 8-2 DMA+AD多通道
		{".."}};

	Menu_RunMenu(Menu_ToolsOptionList);
}

void Menu_RunGamesMenu(void)
{
	static struct Option_Class Menu_GamesOptionList[] = {
		{"<<<"},
		{"Snake", Game_Snake_Init}, // 贪吃蛇
		//	{"Snake II", Game_Snake_Init},	//贪吃蛇2
		//	{"Snake III", Game_Snake_Init},	//贪吃蛇3
		//	{"Snake IV", Game_Snake_Init},	//贪吃蛇4
		//	{"Snake V", Game_Snake_Init},	//贪吃蛇5
		//	{"Snake VI", Game_Snake_Init},	//贪吃蛇6
		//	{"Snake VII", Game_Snake_Init},	//贪吃蛇7
		//	{"Snake VIII", Game_Snake_Init},//贪吃蛇8
		//	{"Snake IX", Game_Snake_Init},	//贪吃蛇9
		//{"Snake X", Game_Snake_Init},		 // 贪吃蛇10
		{"康威生命游戏", Game_Of_Life_Play}, // 康威生命游戏,元胞自动机
		{".."}};

	Menu_RunMenu(Menu_GamesOptionList);
}

void Menu_Information(void)
{
	uint8_t Angle = 233;
	while (1)
	{
		OLED_Clear();

		OLED_ShowImage(88, 8, 32, 32, goutou);
		OLED_Rotation_Block(88 + 16, 8 + 16, 16, Angle * 360 / 256);

		Angle += Menu_RollEvent() * 8;
		Angle += 2;

		OLED_ShowString(2, 0, "Menu V1.8", OLED_8X16);
		OLED_ShowString(2, 32, "By:Adam", OLED_8X16);
		OLED_ShowString(2, 48, "UP:加油哦大灰狼", OLED_8X16);

		OLED_Update();
		if (Menu_EnterEvent())
		{
			return;
		}
		if (Menu_BackEvent())
		{
			return;
		}
	}
}

/**********************************************************/
