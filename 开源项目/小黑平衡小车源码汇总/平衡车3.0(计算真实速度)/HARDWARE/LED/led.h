#ifndef __LED_H
#define __LED_H
#define LED0 GPIO_SetBits(GPIOA,GPIO_Pin_4)   //�رյ�
#define LED1 GPIO_ResetBits(GPIOA,GPIO_Pin_4) //������
void LED_Init(void);
#endif
