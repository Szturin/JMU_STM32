#include "stm32f10x.h"                  // Device header
#include "Menu.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "Delay.h"

/*
本程序采用“事件更新”法，这样在使用OLED菜单时，当且仅当有界面切换或者光标移动时才会刷新OLED
防止对程序造成阻塞
*/

void LED_Menu(void);
void Key_Proc(void);
void Main_Menu(void);
void Control_Menu(void);
void Voltage_Menu(void);
void Breath_Mode(void);
void Control_Proc(void);
void Timer_Menu(void);
unsigned char Cursor=1;//光标
unsigned char Meun_Disp_level[]={0,0};//菜单层级
unsigned int Key_Event;//按键事件
unsigned char KeyNum;//键值

unsigned char PWM_Enable=0;

void Menu_Init(void)//菜单初始化
{
	OLED_ShowString(4,2,"  ");
	OLED_ShowString(2,2,">>");	
	OLED_ShowString(3,2,"  ");	
	Cursor=1;
	Key_Event=0;
	Main_Menu();
}

void Cursor_Init(void)//光标初始化，用于界面切换时对相关参数进行复位
{
	/*光标显示复位*/
	OLED_Clear();//OLED清屏
	OLED_ShowString(4,2,"  ");
	OLED_ShowString(2,2,">>");
	OLED_ShowString(3,2,"  ");
	Cursor=1;//光标值复位
	Key_Event=0;//按键事件复位
}

/**
  * @函	数		按键处理程序
  * @参	数		无
  * @返回值		无
  * @特殊说明	改变“按键事件”Key_event的值	
  */
void Key_Proc(void)
{
	KeyNum=Key_GetNum();	
	if(KeyNum==1)
	{
		Key_Event=2;//光标移位事件
		if(++Cursor==4)
		{
			Cursor=1;
		}
	}
	else if(KeyNum==2)
	{
		Key_Event=1;//确认事件
	}
	else if(KeyNum==3)
	{
		Key_Event=-1;//退出事件
	}
	
	if(Key_Event==2)//仅当按键事件触发，才对OLED进行更新显示,防止阻塞主程序
	{
		Key_Event=0;
		switch(Cursor)
		{
			case 1:
				OLED_ShowString(4,2,"  ");
				OLED_ShowString(2,2,">>");	
				OLED_ShowString(3,2,"  ");
			break;
			case 2:
				OLED_ShowString(2,2,"  ");
				OLED_ShowString(3,2,">>");
				OLED_ShowString(4,2,"  ");
			break;
			case 3:
				OLED_ShowString(3,2,"  ");
				OLED_ShowString(4,2,">>");
				OLED_ShowString(2,2,"  ");
			break;
		}	
	}
}

/*菜单扫描函数*/
void Menu_Proc(void)
{
		switch(Meun_Disp_level[0])
		{
			case 0://在主菜单界面
				if(Key_Event==1)
				{
					Key_Event=0;
					switch(Cursor)
					{
						case 1:
							Cursor_Init();
							LED_Menu();//进入LED菜单（0层，1级)
							Meun_Disp_level[0]=1;
							break;
						case 2:
							Cursor_Init();
							Control_Menu();//进入控制菜单（0层，2级)
							Meun_Disp_level[0]=2;
							break;
						case 3:
							Cursor_Init();
							Voltage_Menu();//进入电压菜单（0层，3级)
							Meun_Disp_level[0]=3;
							break;
					}
				}
				break;
			case 1://位于LED菜单（0层，1级)
				switch(Meun_Disp_level[1])//判断1层级数
				{
					case 0://0层
						if(Key_Event==1)//确认
						{
							Key_Event=0;
							switch(Cursor)
							{
								case 2://光标指向Breath Mode
									Cursor_Init();
									Breath_Mode();//启用Breath Mode菜单
									Meun_Disp_level[1]=1;//1层级数置1
									break;
								case 3:
									Cursor_Init();
									Breath_Mode();//启用Timer Mode菜单
									Meun_Disp_level[1]=2;//1层级数置2
									break;									
							}					
						}			
						else if(Key_Event==-1)//返回
						{
							Key_Event=0;
							Cursor_Init();
							Main_Menu();
							Meun_Disp_level[0]=0;
						}
						break;
					case 1:	
						if(Key_Event==-1)//返回
						{
							Key_Event=0;
							Cursor_Init();
							LED_Menu();
							Meun_Disp_level[1]=0;
						}
						//此按键的作用？ 仅触发有效按键事件的情况下，才会进行OLED更新
						if(Key_Event==1)//确认
						{
							Key_Event=0;
							Breath_Mode();
						}
						break;	
					case 2:
						if(Key_Event==-1)//返回
						{
							Key_Event=0;
							Cursor_Init();
							LED_Menu();
							Meun_Disp_level[1]=0;
						}
						//此按键的作用？ 仅触发有效按键事件的情况下，才会进行OLED更新
						if(Key_Event==1)//确认
						{
							Key_Event=0;
							Timer_Menu();
						}
						break;						
				}
				break;
			case 2:
				if(Key_Event==-1)
				{
					Key_Event=0;
					Cursor_Init();
					Main_Menu();
					Meun_Disp_level[0]=0;
				}
				break;
			case 3:
				if(Key_Event==-1)
				{
					Key_Event=0;
					Cursor_Init();
					Main_Menu();
					Meun_Disp_level[0]=0;
				}
				break;
		}
		Control_Proc();
}

/*菜单显示模块*/
/*主菜单*/
void Main_Menu(void)
{
	OLED_ShowString(1,6,"-MAIN-");
	OLED_ShowString(2,5,"LED Menu");
	OLED_ShowString(3,5,"Control Menu");
	OLED_ShowString(4,5,"Voltage Menu");
	
}

/*LED菜单*/
void LED_Menu(void)
{
	OLED_ShowString(1,6,"-LED-");
	OLED_ShowString(2,5,"Flowing Mode");
	OLED_ShowString(3,5,"Breath Mode");
	OLED_ShowString(4,5,"Timer Mode");
}

void Breath_Mode()
{
	OLED_ShowString(2,5,"Enable:");
	OLED_ShowString(3,5,"Light:  %");
	OLED_ShowNum(2,12,PWM_Enable,1);
	if(Meun_Disp_level[1]==1)//菜单扫描函数中是先更新显示，再次切换菜单层级的，这句IF的作用是防止未进入此界面就执行一遍下面的语句
	{
		switch(Cursor)
		{
			case 1:
				PWM_Enable^=1;
				break;
		}
		OLED_ShowNum(2,12,PWM_Enable,1);		
	}
}

void Timer_Menu(void)
{
	OLED_ShowString(2,5,"T:    ms");
	OLED_ShowString(3,5,"Time:  ");
	OLED_ShowNum(3,6,Timer,1);	
}

/*外设控制处理函数*/
void Control_Proc(void)
{
	static unsigned int i=0;
	
	/*呼吸灯模块*/
	if(PWM_Enable==1)
	{
		PWM_Init();
		
		if(i<800)
		{
			i++;
			PWM_SetCompare1(i/8);
			OLED_ShowNum(3,11,i/8,2);
		}
		if(i>=800)
		{
			i++;
			PWM_SetCompare1(200-i/8);
			OLED_ShowNum(3,11,200-i/8,2);
			if(i>=1600){i=0;}
		}
		
	}
	else
	{
		PWM_SetCompare1(0);
	}
	
	
	
}
/*控制菜单*/
void Control_Menu(void)
{
	OLED_ShowString(1,4,"-Control-");
	OLED_ShowString(2,5,"Motor");
	OLED_ShowString(3,5,"Servo");
	OLED_ShowString(4,5,"...");
}

/*电压菜单*/
void Voltage_Menu(void)
{
	OLED_ShowString(1,4,"-Voltage-");
	OLED_ShowString(2,5,"AD MODE");
	OLED_ShowString(3,5,"DAC MODE");
	OLED_ShowString(4,5,"...");
}