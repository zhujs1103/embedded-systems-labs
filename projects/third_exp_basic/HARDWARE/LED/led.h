#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

#define LED_SET(dat) GPIO_Write(GPIOF, (dat))

extern volatile uint8_t led_flow_flag;

void LED_Hardware_Init(void);
void LED_MainFlowStep(void);
void LED_TrafficFlash(void);
void LED_Flow(void);

#endif
