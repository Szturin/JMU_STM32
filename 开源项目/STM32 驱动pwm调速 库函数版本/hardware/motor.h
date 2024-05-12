
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void TIM2_PWM_Init(void);

void MOTOR_GPIO_Config(void);
void TIM2_PWM_CHANGE(u16 CCR3,u16 CCR4);
void LED_init(void);



extern int leftcount;      
extern int rightcount;     

#endif
