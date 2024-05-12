#include <stm32f10x.h> 			//Device header
#include <Delay.h>
#include <OLED.h>
#include <Menu.h>
#include <Key.h>

unsigned char i;
unsigned char temp;
int main(void)
{
	Key_Init();
	OLED_Init();
	Menu_Init();
	while(1)
	{
		Menu_Proc();
		Key_Proc();
	}
}
