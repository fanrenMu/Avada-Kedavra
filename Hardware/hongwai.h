#ifndef __HONGWAI_H
#define __HONGWAI_H


#include "stm32f10x.h"
#include "Delay.h"

void PWM_Init(void);
void IR_ON(void);
void IR_OFF(void);
void IR_Send_Raw(uint16_t *data, uint16_t length);
#endif
