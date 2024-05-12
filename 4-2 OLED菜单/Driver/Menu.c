#include "stm32f10x.h"                  // Device header
#include "Menu.h"
#include "OLED.h"
#include "Key.h"

/*
��������á��¼����¡�����������ʹ��OLED�˵�ʱ�����ҽ����н����л����߹���ƶ�ʱ�Ż�ˢ��OLED
��ֹ�Գ����������
*/

void LED_Menu(void);
void Key_Proc(void);
void Main_Menu(void);
void Control_Menu(void);
void Voltage_Menu(void);

unsigned char Cursor=1;//���
unsigned char Meun_Disp_level[]={0,0};//�˵��㼶
unsigned int Key_Event;//�����¼�
unsigned char KeyNum;//��ֵ

void Menu_Init(void)//�˵���ʼ��
{
	OLED_ShowString(4,2,"  ");
	OLED_ShowString(2,2,">>");	
	OLED_ShowString(3,2,"  ");	
	Cursor=1;
	Key_Event=0;
	Main_Menu();
}

void Cursor_Init(void)//����ʼ�������ڽ����л�ʱ����ز������и�λ
{
	/*�����ʾ��λ*/
	OLED_Clear();//OLED����
	OLED_ShowString(4,2,"  ");
	OLED_ShowString(2,2,">>");
	OLED_ShowString(3,2,"  ");
	Cursor=1;//���ֵ��λ
	Key_Event=0;//�����¼���λ
}

void Key_Proc(void)
{
	KeyNum=Key_GetNum();	
	if(KeyNum==1)
	{
		Key_Event=2;//�����λ�¼�
		if(++Cursor==4)
		{
			Cursor=1;
		}
	}
	else if(KeyNum==2)
	{
		Key_Event=1;//ȷ���¼�
	}
	else if(KeyNum==3)
	{
		Key_Event=-1;//�˳��¼�
	}
	
	if(Key_Event==2)//���������¼��������Ŷ�OLED���и�����ʾ,��ֹ����������
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

/*�˵�ɨ�躯��*/
void Menu_Proc(void)
{
	switch(Meun_Disp_level[0])
	{
		case 0:
			if(Key_Event==1)
			{
				switch(Cursor)
				{
					case 1:
						Cursor_Init();
						LED_Menu();
						Meun_Disp_level[0]=1;
						break;
					case 2:
						Cursor_Init();
						Control_Menu();
						Meun_Disp_level[0]=2;
						break;
					case 3:
						Cursor_Init();
						Voltage_Menu();
						Meun_Disp_level[0]=3;
						break;
				}
			}
			break;
		case 1:
			if(Key_Event==-1)
			{
				Cursor_Init();
				Main_Menu();
				Meun_Disp_level[0]=0;
			}
			break;
		case 2:
			if(Key_Event==-1)
			{
				Cursor_Init();
				Main_Menu();
				Meun_Disp_level[0]=0;
			}
			break;
		case 3:
			if(Key_Event==-1)
			{
				Cursor_Init();
				Main_Menu();
				Meun_Disp_level[0]=0;
			}
			break;
	}
}

/*�˵���ʾģ��*/
/*���˵�*/
void Main_Menu(void)
{
	OLED_ShowString(1,6,"-MAIN-");
	OLED_ShowString(2,5,"LED Menu");
	OLED_ShowString(3,5,"Control Menu");
	OLED_ShowString(4,5,"Voltage Menu");
	
}

/*LED�˵�*/
void LED_Menu(void)
{
	OLED_ShowString(1,6,"-LED-");
	OLED_ShowString(2,5,"Flowing Mode");
	OLED_ShowString(3,5,"Breath Mode");
	OLED_ShowString(4,5,"...");
}

/*���Ʋ˵�*/
void Control_Menu(void)
{
	OLED_ShowString(1,4,"-Control-");
	OLED_ShowString(2,5,"Motor");
	OLED_ShowString(3,5,"Servo");
	OLED_ShowString(4,5,"...");
}

/*��ѹ�˵�*/
void Voltage_Menu(void)
{
	OLED_ShowString(1,4,"-Voltage-");
	OLED_ShowString(2,5,"AD MODE");
	OLED_ShowString(3,5,"DAC MODE");
	OLED_ShowString(4,5,"...");
}