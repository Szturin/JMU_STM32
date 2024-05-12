#include "stm32f10x.h"                  // Device header
#include "Menu.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "Delay.h"

/*
��������á��¼����¡�����������ʹ��OLED�˵�ʱ�����ҽ����н����л����߹���ƶ�ʱ�Ż�ˢ��OLED
��ֹ�Գ����������
*/

void LED_Menu(void);
void Key_Proc(void);
void Main_Menu(void);
void Control_Menu(void);
void Voltage_Menu(void);
void Breath_Mode(void);
void Control_Proc(void);
void Timer_Menu(void);
unsigned char Cursor=1;//���
unsigned char Meun_Disp_level[]={0,0};//�˵��㼶
unsigned int Key_Event;//�����¼�
unsigned char KeyNum;//��ֵ

unsigned char PWM_Enable=0;

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

/**
  * @��	��		�����������
  * @��	��		��
  * @����ֵ		��
  * @����˵��	�ı䡰�����¼���Key_event��ֵ	
  */
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
			case 0://�����˵�����
				if(Key_Event==1)
				{
					Key_Event=0;
					switch(Cursor)
					{
						case 1:
							Cursor_Init();
							LED_Menu();//����LED�˵���0�㣬1��)
							Meun_Disp_level[0]=1;
							break;
						case 2:
							Cursor_Init();
							Control_Menu();//������Ʋ˵���0�㣬2��)
							Meun_Disp_level[0]=2;
							break;
						case 3:
							Cursor_Init();
							Voltage_Menu();//�����ѹ�˵���0�㣬3��)
							Meun_Disp_level[0]=3;
							break;
					}
				}
				break;
			case 1://λ��LED�˵���0�㣬1��)
				switch(Meun_Disp_level[1])//�ж�1�㼶��
				{
					case 0://0��
						if(Key_Event==1)//ȷ��
						{
							Key_Event=0;
							switch(Cursor)
							{
								case 2://���ָ��Breath Mode
									Cursor_Init();
									Breath_Mode();//����Breath Mode�˵�
									Meun_Disp_level[1]=1;//1�㼶����1
									break;
								case 3:
									Cursor_Init();
									Breath_Mode();//����Timer Mode�˵�
									Meun_Disp_level[1]=2;//1�㼶����2
									break;									
							}					
						}			
						else if(Key_Event==-1)//����
						{
							Key_Event=0;
							Cursor_Init();
							Main_Menu();
							Meun_Disp_level[0]=0;
						}
						break;
					case 1:	
						if(Key_Event==-1)//����
						{
							Key_Event=0;
							Cursor_Init();
							LED_Menu();
							Meun_Disp_level[1]=0;
						}
						//�˰��������ã� ��������Ч�����¼�������£��Ż����OLED����
						if(Key_Event==1)//ȷ��
						{
							Key_Event=0;
							Breath_Mode();
						}
						break;	
					case 2:
						if(Key_Event==-1)//����
						{
							Key_Event=0;
							Cursor_Init();
							LED_Menu();
							Meun_Disp_level[1]=0;
						}
						//�˰��������ã� ��������Ч�����¼�������£��Ż����OLED����
						if(Key_Event==1)//ȷ��
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
	OLED_ShowString(4,5,"Timer Mode");
}

void Breath_Mode()
{
	OLED_ShowString(2,5,"Enable:");
	OLED_ShowString(3,5,"Light:  %");
	OLED_ShowNum(2,12,PWM_Enable,1);
	if(Meun_Disp_level[1]==1)//�˵�ɨ�躯�������ȸ�����ʾ���ٴ��л��˵��㼶�ģ����IF�������Ƿ�ֹδ����˽����ִ��һ����������
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

/*������ƴ�����*/
void Control_Proc(void)
{
	static unsigned int i=0;
	
	/*������ģ��*/
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