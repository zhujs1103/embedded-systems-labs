#ifndef __MY_EXTI_H
#define __MY_EXTI_H

#include "stm32f4xx.h"

extern volatile uint8_t key1_event;
extern volatile uint8_t key2_event;

void EXTI_Configure(void);
uint8_t KEY1_IsDown(void);
uint8_t KEY2_IsDown(void);

#endif
