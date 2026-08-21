#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

#define LED_SET(dat) GPIO_Write(GPIOF, (dat))

extern volatile uint8_t led_flow_flag;

void LED_Hardware_Init(void);
void LED_Flow(void);
void LED_FlowForMs(uint16_t ms);
void LED_TrafficFlash(void);

#endif
