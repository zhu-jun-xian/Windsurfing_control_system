#ifndef __PWM__H
#define __PWM__H
#include "sys.h"
#include "stm32f10x.h"
#define L298N_IN1 PAin(3) 
#define L298N_IN2 PAin(4) 
void TIM4_PWM_Init(u16 arr,u16 psc);  
void L298N_Init(void);
#endif
