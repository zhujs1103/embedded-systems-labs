#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

extern volatile uint8_t led_flow_flag;

void LED_Hardware_Init(void);
void LED_Set(uint16_t data);
void LED_ShowScore(uint8_t score);
void LED_BlinkAll(uint8_t times, uint16_t delay_ms);

#endif
