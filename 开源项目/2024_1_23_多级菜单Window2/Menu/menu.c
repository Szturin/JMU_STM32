#include "Menu.h"

/**********************************************************
 * 单片机多级菜单窗口版	By:Adam
 *
 * 本次更新:
 * 实用工具PWM输出, 可以方便的设置频率和占空比, 大家手里有示波器的帮忙测试一下, 输出引脚:PA0;
 * 优化编码器逻辑, 转动更跟手;
 * 添加OLED补丁,OLED显示图像函数Y轴可以是负数,解决菜单滚动时首行是空白的问题;
 * 优化变量名, 以标准库风格命名变量;
 * 添加了菜单全局属性结构体;
 * 用函数封装了部分功能;
 * 程序结构更清晰;
 * 更多的注释!;
 *
 * 帧率显示,可以跳转到 OLED_Update() 函数解除注释开启;
 * OLED显示函数以江协科技的OLED显示屏驱动程序为基础进行了少量修改
 * OLED显示屏使用软件IIC通信: SCL接到PB8, SDA接到PB9;
 *
 * 更新链接
 * 百度网盘:https://pan.baidu.com/s/1bZPWCKaiNbb-l1gpAv6QNg?pwd=KYWS
 * GitHub:https://github.com/AdamLoong/Embedded_Menu_Simple
 * B站UP:加油哦大灰狼
 * 如果觉得此菜单程序对你有帮助记得回来给个一键三连哦( •̀ ω •́ )✧
 **********************************************************/

/*菜单用到的按键函数独立出来,方便移植和修改,比如没有编码器可以用上下两个按键代替;*/
int8_t Menu_RollEvent(void) // 菜单滚动
{
	if (Key_Up_Get()) // 按键上接到PB15;
	{
		return 1;
	}
	if (Key_Down_Get()) // 按键下接到PB13;
	{
		return -1;
	}
	return Encoder_Get_Div4(); // 旋钮编码器PA8,PA9;
}
int8_t Menu_EnterEvent(void) // 菜单确认
{
	return Key_Enter_Get(); // 确认键接到PB14;
}
int8_t Menu_BackEvent(void) // 菜单返回
{
	return Key_Back_Get(); // 返回键接到PB12;
}

/*菜单全局属性*/
struct MenuProperty Menu_Global = {
	.Cursor_Actual_X = 0,  // 当前光标位置X
	.Cursor_Actual_Y = 63, // 当前光标位置Y
	.Cursor_Actual_W = 0,  // 当前光标尺寸宽
	.Cursor_Actual_H = 0,  // 当前光标尺寸高

	.Cur_Style = reverse,	 // 光标风格;
	.Cursor_ActSpeed = 0.15, // 光标动画速度系数;
	.Slide_ActSpeed = 2,	 // 滚动动画速度系数;

	.Font_Width = 6,	// 字体宽度 8 或 6
	.Font_Height = 8,	// 字体高度
	.Line_Height = 12,	// 行高
	.Layout_Margin = 2, // 页边距

	.Window_X = 0,	 // 窗口位置X 面向对象
	.Window_Y = 0,	 // 窗口位置Y
	.Window_W = 128, // 窗口宽度
	.Window_H = 64,	 // 窗口高度

	.RunningMenu = &Menu_StartMenu, // 运行中的菜单
};

/*菜单对象初始化*/
void Menu_MenuClassInit(struct Menu_Class *MU, struct Option_Class *Option_List)
{
	Menu_Global.RunningMenu->isReadKey = 0; // 关闭上级读取按键
	Menu_Global.RunningMenu->isCaller = 1;	// 启动上级调用下级程序

	MU->isInit = 0;	   // 下次循环不初始化了
	MU->isShow = 1;	   // 是显示菜单
	MU->isReadKey = 1; // 是读取按键
	MU->isCaller = 0;  // 否调用下级程序

	MU->Cat_i = 1;		   // 选中下标
	MU->Cur_i = 0;		   // 光标下标
	MU->Cur_i_Ceiling = 0; // 光标限位

	MU->Show_i = 0;			 // 显示起始下标
	MU->Show_i_previous = 0; // 显示起始下标的前一个状态(用于比较)
	MU->Show_offset = 0;	 // 显示Y轴的偏移

	/*继承全局菜单状态*/
	MU->Cur_Style = Menu_Global.Cur_Style;		   // 光标风格
	MU->Font_Width = Menu_Global.Font_Width;	   // 字宽
	MU->Font_Height = Menu_Global.Font_Height;	   // 字高
	MU->Line_Height = Menu_Global.Line_Height;	   // 行高
	MU->Layout_Margin = Menu_Global.Layout_Margin; // 页边距

	MU->Window_Border = 0; // 窗口边框
	MU->Window_BotCol = 0; // 窗口底色bottom colour

	MU->Option_List = Option_List; // 指向选项列表

	/* 计算选项列表长度, 各选项名字长度, 最长选项名字长度*/
	for (MU->Option_MaxNum = 0, MU->Option_MaxStrLen = 0; MU->Option_List[MU->Option_MaxNum].String[0] != '.'; MU->Option_MaxNum++)
	{
		MU->Option_List[MU->Option_MaxNum].StrLen = Menu_GetOptionStrLen(MU->Option_List[MU->Option_MaxNum].String); // 顺便计算选项名字长度
		if (MU->Option_List[MU->Option_MaxNum].StrLen > MU->Option_MaxStrLen)
		{
			MU->Option_MaxStrLen = MU->Option_List[MU->Option_MaxNum].StrLen; // 顺便计算最长选项名字长度
		}
	}
	MU->Option_MaxNum--; // 不显示结尾标志".."

	/*窗口从光标位置展开*/
	MU->Window_X = Menu_Global.Cursor_Actual_X + Menu_Global.Cursor_Actual_W;
	MU->Window_Y = Menu_Global.Cursor_Actual_Y;
	MU->Window_W = 0;
	MU->Window_H = Menu_Global.Cursor_Actual_H;
	/*计算窗口目标位置尺寸*/
	MU->Target_Window_W = (MU->Option_MaxStrLen * MU->Font_Width) + (MU->Layout_Margin * 3);
	MU->Target_Window_H = ((MU->Option_MaxNum > 4) ? (4) : (MU->Option_MaxNum)) * MU->Line_Height + (MU->Layout_Margin * 2);
	MU->Target_Window_X = (((Menu_Global.Cursor_Actual_X + Menu_Global.Cursor_Actual_W) + MU->Target_Window_W) > 128) ? (128 - MU->Target_Window_W) : (Menu_Global.Cursor_Actual_X + Menu_Global.Cursor_Actual_W);
	MU->Target_Window_Y = ((Menu_Global.Cursor_Actual_Y + MU->Target_Window_H) > 64) ? (64 - MU->Target_Window_H) : (Menu_Global.Cursor_Actual_Y);

	Menu_Global.RunningMenu = MU; // 当前运行菜单变成当前初始化菜单
}

/**
 * 函    数：菜单运行界面窗口化
 * 参    数：选项列表
 * 返 回 值：菜单运行状态: 0:不运行, 1:正常运行, >=2:多级运行并表示层级数量
 * 说    明：把选项列表显示出来,并根据按键事件执行相应操作
 */
int8_t Menu_RunWindow(struct Menu_Class *MU)
{
	int8_t Roll_Event = 0;

	if (MU->isReadKey) // 是否读取按键
	{
		Roll_Event = Menu_RollEvent();

		if (Menu_EnterEvent())
		{
			/*如果功能不为空则执行功能,否则返回*/
			if (MU->Option_List[MU->Cat_i].func)
			{
				MU->Option_List[MU->Cat_i].func();
			}
			else
			{
				MU->isInit = 1;
				return 0;
			}
		}
		if (Menu_BackEvent())
		{
			MU->isInit = 1; // 退出菜单后下次进入初始化菜单状态
			return 0;
		}
	}

	/**********************************************************/

	if (MU->isShow) // 是否显示菜单
	{
		/*窗口平滑缩放*/
		MU->Window_X = Menu_CurveMigration(MU->Window_X, MU->Target_Window_X, Menu_Global.Cursor_ActSpeed);
		MU->Window_Y = Menu_CurveMigration(MU->Window_Y, MU->Target_Window_Y, Menu_Global.Cursor_ActSpeed);
		MU->Window_W = Menu_CurveMigration(MU->Window_W, MU->Target_Window_W, Menu_Global.Cursor_ActSpeed);
		MU->Window_H = Menu_CurveMigration(MU->Window_H, MU->Target_Window_H, Menu_Global.Cursor_ActSpeed);

		OLED_ClearArea(MU->Window_X, MU->Window_Y, MU->Window_W, MU->Window_H); // 区域清除
		/*光标下标限制等于窗口高度减去上下页边距再除以行高*/
		MU->Cur_i_Ceiling = (MU->Window_H - MU->Layout_Margin * 2) / MU->Line_Height; // 计算光标限制位置;

		/*根据按键事件更改选中下标和光标下标*/
		if (Roll_Event)
		{
			/*更新下标*/
			MU->Cur_i += Roll_Event;
			MU->Cat_i += Roll_Event;
			/*限制选中下标*/
			if (MU->Cat_i > MU->Option_MaxNum)
			{
				MU->Cat_i = MU->Option_MaxNum;
			}
			if (MU->Cat_i < 0)
			{
				MU->Cat_i = 0;
			}
			/*限制光标下标*/
			if (!(MU->Cur_i < MU->Cur_i_Ceiling))
			{
				MU->Cur_i = MU->Cur_i_Ceiling - 1;
			}
			if (MU->Cur_i > MU->Option_MaxNum)
			{
				MU->Cur_i = MU->Option_MaxNum;
			}
			if (MU->Cur_i < 0)
			{
				MU->Cur_i = 0;
			}
		}

		/**********************************************************/

		/*计算显示起始下标*/
		MU->Show_i = MU->Cat_i - MU->Cur_i;

		if (1) // 增加显示偏移量实现平滑移动
		{
			if (MU->Show_i - MU->Show_i_previous) // 如果下标有偏移
			{
				MU->Show_offset = (MU->Show_i - MU->Show_i_previous) * MU->Line_Height; // 计算显示偏移量
				MU->Show_i_previous = MU->Show_i;										// 记录
			}
			if (MU->Show_offset)
			{
				MU->Show_offset /= Menu_Global.Slide_ActSpeed; // 显示偏移量逐渐归零
			}
		}

		for (int8_t i = 0; i < MU->Cur_i_Ceiling; i++) // 遍历显示选项
		{
			if (MU->Show_i + i > MU->Option_MaxNum)
			{
				break;
			}

			/*范围打印函数会返回打印的字符串长度复制给选项长度*/
			MU->Option_List[MU->Show_i + i].StrLen =
				/*使用格式化字符串打印, 支持添加一个(float)变量*/
				Menu_PrintfOptionStr(
					/*显示从窗口X起点, 加上页边距*/
					MU->Window_X + MU->Layout_Margin,
					/*显示从窗口Y起点, 加上页边距, 加上行偏移, 加上显示偏移, 加上垂直居中*/
					MU->Window_Y + MU->Layout_Margin + (i * MU->Line_Height) + (MU->Show_offset) + ((MU->Line_Height - MU->Font_Height) / 2),
					/*显示宽度范围减去双倍(左右)页边距*/
					MU->Window_W - MU->Layout_Margin * 2,
					/*显示高度就是行高(或字高)*/
					MU->Line_Height,
					/*显示字符的宽度*/
					MU->Font_Width,
					/*要显示的字符串*/
					MU->Option_List[MU->Show_i + i].String,
					/*格式化字符串的一个变量*/
					*MU->Option_List[MU->Show_i + i].Variable);
		}

		OLED_DrawRectangle(MU->Window_X, MU->Window_Y, MU->Window_W, MU->Window_H, 0); // 显示窗口边框
	}
	if (MU->isCaller) // 是否调用下级程序
	{
		MU->Option_List[MU->Cat_i].func();	 // 去喊下级菜单干活
		if (Menu_Global.RunningMenu->isInit) // 如果当前运行菜单(下级)处于需要初始化状态
		{
			MU->isReadKey = 1;			  // 我的按键读取启动
			MU->isCaller = 0;			  // 我的调用下级关闭
			Menu_Global.RunningMenu = MU; // 当前运行菜单变成我(●'◡'●)
		}
	}
	else
	{
		// 调用显示光标函数
		Menu_ShowCursor(MU->Window_X + MU->Layout_Margin,										// 光标左起点加上页边距
						MU->Window_Y + MU->Layout_Margin + (MU->Cur_i * MU->Line_Height),		// 光标上起点加上光标行偏移
						MU->Option_List[MU->Cat_i].StrLen * MU->Font_Width + MU->Layout_Margin, // 光标宽度 等于字符串长度乘以字符宽度加上页边距(跟随字符串)
						/*MU->Window_W - MU->Layout_Margin * 2,	*/								// 光标宽度 等于窗口宽度减去左右页边距(等长)(二选一注释)
						MU->Line_Height,														// 光标高度就是行高
						MU->Cur_Style,															// 光标状态
						Menu_Global.Cursor_ActSpeed);											// 光标速度
	}

	/**********************************************************/
	/*调试信息*/

	// OLED_ShowSignedNum(110, 48, Cur_i, 2, OLED_6X8);
	// OLED_ShowSignedNum(110, 56, Cat_i, 2, OLED_6X8);

	// int delay = 1000000; while(delay--);
	/**********************************************************/

	return 1;
}

/**
 * 函    数：计算选项字符串长度;
 * 参    数：String 计算长度的字符串
 * 返 回 值：字符串长度
 * 说    明：英文一个长度,中文两个长度;
 */
uint8_t Menu_GetOptionStrLen(char *String)
{
	uint8_t i = 0, len = 0;
	while (String[i] != '\0') // 遍历字符串的每个字符
	{
		if (String[i] > '~') // 如果不属于英文字符长度加2
		{
			len += 2;
			i += 3;
		}
		else // 属于英文字符长度加1
		{
			len += 1;
			i += 1;
		}
	}
	return len;
}

/**
 * 函    数：使用printf函数打印格式化字符串(增加范围打印)
 * 参    数：X 指定格式化字符串左上角的横坐标，范围：0~127
 * 参    数：Y 指定格式化字符串左上角的纵坐标，范围：0~63
 * 参    数：Width  范围：0~127
 * 参    数：Height 范围：0~63
 * 参    数：FontSize 指定字体大小
 *           范围：OLED_8X16		宽8像素，高16像素
 *                 OLED_6X8		宽6像素，高8像素
 * 参    数：format 指定要显示的格式化字符串，范围：ASCII码可见字符组成的字符串
 * 参    数：... 格式化字符串参数列表
 * 返 回 值：*****************打印字符串的长度******************
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
uint8_t Menu_PrintfOptionStr(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t FontSize, char *format, ...)
{
	char String[30];												   // 定义字符数组
	va_list arg;													   // 定义可变参数列表数据类型的变量arg
	va_start(arg, format);											   // 从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);									   // 使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);													   // 结束变量arg
	return OLED_ShowStringArea(X, Y, Width, Height, String, FontSize); // OLED显示字符数组（字符串）并返回字符串
}

/**
 * 函    数：平滑偏移
 * 参    数：Actual_Value 当前数值
 * 参    数：Target_Value 目标数值
 * 参    数：Act_Speed 偏移速度
 * 返 回 值: 当前数值偏移后的值
 * 说    明：按照格式写入参数, 每次循环调用, 当前值都向目标值靠近一定距离
 */
float Menu_CurveMigration(float Actual_Value, float Target_Value, float Act_Speed)
{
	if ((Target_Value - Actual_Value) > 1)
	{
		Actual_Value += (Target_Value - Actual_Value) * Act_Speed + 1;
	}
	else if ((Target_Value - Actual_Value) < -1)
	{
		Actual_Value += (Target_Value - Actual_Value) * Act_Speed - 1;
	}
	else
	{
		Actual_Value = Target_Value;
	}

	return Actual_Value;
}

/**
 * 函    数：菜单平滑显示光标
 * 参    数：Target_Cur_X 光标目标X位置
 * 参    数：Target_Cur_Y 光标目标Y位置
 * 参    数：Target_Cur_W 光标目标宽度
 * 参    数：Target_Cur_H 光标目标高度
 * 参    数：Cur_Act_Speed 光标移动速度范围: 0 < Cur_Act_Speed <=1
 * 返 回 值：无
 * 说    明：输入光标目标位置和尺寸, 光标会在多次打印中逐渐接近目标位置和尺寸;
 */
void Menu_ShowCursor(float Target_Cur_X, float Target_Cur_Y, float Target_Cur_W, float Target_Cur_H, enum CursorStyle CurStyle, float Cur_Act_Speed)
{
	Menu_Global.Cursor_Actual_X = Menu_CurveMigration(Menu_Global.Cursor_Actual_X, Target_Cur_X, Cur_Act_Speed);
	Menu_Global.Cursor_Actual_Y = Menu_CurveMigration(Menu_Global.Cursor_Actual_Y, Target_Cur_Y, Cur_Act_Speed);
	Menu_Global.Cursor_Actual_W = Menu_CurveMigration(Menu_Global.Cursor_Actual_W, Target_Cur_W, Cur_Act_Speed);
	Menu_Global.Cursor_Actual_H = Menu_CurveMigration(Menu_Global.Cursor_Actual_H, Target_Cur_H, Cur_Act_Speed);

	if (CurStyle == reverse)
	{
		OLED_ReverseArea(Menu_Global.Cursor_Actual_X, Menu_Global.Cursor_Actual_Y, Menu_Global.Cursor_Actual_W, Menu_Global.Cursor_Actual_H); // 反相光标
	}
	else if (CurStyle == mouse)
	{
		// OLED_ShowString(Menu_Global.Cursor_Actual_X + Menu_Global.Cursor_Actual_W, Menu_Global.Cursor_Actual_Y + (Menu_Global.Cursor_Actual_H - 6) / 2, "<-", OLED_6X8); // 尾巴光标
		OLED_ShowImage(Menu_Global.Cursor_Actual_X + Menu_Global.Cursor_Actual_W, Menu_Global.Cursor_Actual_Y + (Menu_Global.Cursor_Actual_H - 6) / 2, 9, 7, Cursor);
	}
	else if (CurStyle == frame)
	{
		OLED_DrawRectangle(Menu_Global.Cursor_Actual_X, Menu_Global.Cursor_Actual_Y, Menu_Global.Cursor_Actual_W, Menu_Global.Cursor_Actual_H, 0); // 矩形光标
	}
}

/***
 * 函    数: 显示壁纸;
 * 参    数: 图片库的壁纸
 * 返 回 值:
 * 说    明: 使用memcpy高效率把壁纸数据复制到显存;
 */
void Menu_ShowWallpaper(const uint8_t *Wallpaper)
{
	memcpy(OLED_DisplayBuf, Wallpaper, 1024);
}

/***
 * 函    数: 显示坤坤;
 * 参    数: 图片库的壁纸
 * 返 回 值:
 * 说    明: 使用memcpy高效率把壁纸数据复制到显存;
 */
void Menu_Showkunkun(void)
{
	static uint8_t count = 0;
	
	memcpy(OLED_DisplayBuf, kunkun[count++], 512);
	count %= 13;
}

/**
 * 函    数：运行简单列表
 * 参    数：选项列表
 * 返 回 值：
 * 说    明：把选项列表显示出来,并根据按键事件执行相应操作
 */
void Menu_Run_Option_List(struct Option_Class *Option_List)
{

	int8_t Roll_Event = 0;		// 记录菜单滚动事件
	int8_t Show_i = 0;			// 显示起始下标
	int8_t Show_i_previous = 4; // 显示起始下标的前一个状态(用于比较)
	int8_t Show_offset;			// 显示Y轴的偏移
	int8_t Cat_i = 1;			// 选中下标默认为1,(因为Option_List[0]为"<<<")
	int8_t Cur_i = 0;			// 光标下标默认为0

	int8_t Option_MaxNum = 0;
	for (Option_MaxNum = 0; Option_List[Option_MaxNum].String[0] != '.'; Option_MaxNum++) // 计算选项列表长度
	{
		// Option_List[Option_MaxNum].StrLen = Menu_GetOptionStrLen(Option_List[Option_MaxNum].String); // 顺手计算选项名字长度
	}
	Option_MaxNum--;

	/*光标下标限制等于窗口高度减去上下页边距再除以行高*/
	int8_t Cur_i_Ceiling = (Menu_Global.Window_H - Menu_Global.Layout_Margin * 2) / Menu_Global.Line_Height; // 计算光标限制位置;

	/**********************************************************/

	while (1)
	{

		if (Menu_EnterEvent())
		{
			/*如果功能不为空则执行功能,否则返回*/
			if (Option_List[Cat_i].func)
			{
				// Option_List[Cat_i].func(0);
			}
			else
			{
				return;
			}
		}
		if (Menu_BackEvent())
		{
			return;
		}

		/*根据按键事件更改选中下标和光标下标*/
		Roll_Event = Menu_RollEvent();
		if (Roll_Event)
		{
			/*更新下标*/
			Cur_i += Roll_Event;
			Cat_i += Roll_Event;
			/*限制选中下标*/
			if (Cat_i > Option_MaxNum)
			{
				Cat_i = Option_MaxNum;
			}
			if (Cat_i < 0)
			{
				Cat_i = 0;
			}
			/*限制光标下标*/
			if (Cur_i >= Cur_i_Ceiling)
			{
				Cur_i = Cur_i_Ceiling - 1;
			}
			if (Cur_i > Option_MaxNum)
			{
				Cur_i = Option_MaxNum;
			}
			if (Cur_i < 0) // 踩坑记录: (Cur_i >= Cur_i_Ceiling) Cur_i_Ceiling 有可能是负数, 如果放在(Cur_i < 0)后面判断, 则 Cur_i 会变成负数, 造成程序卡死; 结论: 如果进行位置限制判断, 变量判断应该放在前,常量判断应该放在后;
			{
				Cur_i = 0;
			}
		}

		/**********************************************************/

		OLED_Clear();

		/*计算显示起始下标*/
		Show_i = Cat_i - Cur_i;

		if (1) // 增加显示偏移量实现平滑移动
		{
			if (Show_i - Show_i_previous) // 如果下标有偏移
			{
				Show_offset = (Show_i - Show_i_previous) * Menu_Global.Line_Height; // 计算显示偏移量
				Show_i_previous = Show_i;
			}
			if (Show_offset)
			{
				Show_offset /= Menu_Global.Slide_ActSpeed; // 显示偏移量逐渐归零
			}
		}

		for (int8_t i = -1; i < Cur_i_Ceiling + 1; i++) // 遍历显示选项(遍历 i 从 -1 开始到 Cur_i_Ceiling + 1 结束, 是为了菜单滚动的时候首行和尾行不会有空白);
		{
			if (Show_i + i < 0)
			{
				continue;
			}
			if (Show_i + i > Option_MaxNum)
			{
				break;
			}

			/*菜单格式化打印函数会返回打印的字符串长度*/
			Option_List[Show_i + i].StrLen =
				/*使用格式化字符串打印, 支持添加一个(float)变量*/
				Menu_PrintfOptionStr(
					/*显示从窗口X起点, 加上页边距*/
					2 + Menu_Global.Window_X + Menu_Global.Layout_Margin,
					/*显示从窗口Y起点, 加上页边距, 加上行偏移, 加上显示偏移, 加上垂直居中*/
					Menu_Global.Window_Y + (Menu_Global.Layout_Margin) + (i * Menu_Global.Line_Height) + (Show_offset) + ((Menu_Global.Line_Height - Menu_Global.Font_Height) / 2),
					/*显示宽度范围减去双倍(左右)页边距*/
					Menu_Global.Window_W - Menu_Global.Layout_Margin * 2,
					/*显示高度就是行高(或字高)*/
					Menu_Global.Line_Height,
					/*显示字符的宽度*/
					Menu_Global.Font_Width,
					/*要显示的字符串*/
					Option_List[Show_i + i].String,
					/*可选变量*/
					*Option_List[Show_i + i].Variable);
		}

		// 调用显示光标函数
		Menu_ShowCursor(Menu_Global.Window_X + Menu_Global.Layout_Margin,									  // 光标左起点加上页边距
						Menu_Global.Window_Y + Menu_Global.Layout_Margin + (Cur_i * Menu_Global.Line_Height), // 光标上起点加上光标行偏移
						4 + Option_List[Cat_i].StrLen * Menu_Global.Font_Width + Menu_Global.Layout_Margin,	  // 光标宽度等于字符串长度乘以字符宽度加上页边距(跟随字符串)
																											  // Menu_Global.Window_W - Menu_Global.Layout_Margin * 2,// 光标宽度等于窗口宽度减去左右页边距(等长)(二选一注释)
						Menu_Global.Line_Height,															  // 光标高度就是行高
						Menu_Global.Cur_Style,																  // 光标状态
						Menu_Global.Cursor_ActSpeed);														  // 光标速度

		// OLED_DrawRectangle(Menu_Global.Window_X, Menu_Global.Window_Y, Menu_Global.Window_W, Menu_Global.Window_H, 0); // 显示窗口边框,根据个人喜好选择

		/**********************************************************/
		/*调试信息*/

		// OLED_ShowSignedNum(110, 48, Cur_i, 2, OLED_6X8);
		// OLED_ShowSignedNum(110, 56, Cat_i, 2, OLED_6X8);

		// int delay = 1000000; while(delay--);
		/**********************************************************/
		OLED_Update();
	}
}
