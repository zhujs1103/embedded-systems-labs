#ifndef __SMG_H_
#define __SMG_H_

#include "stm32f4xx.h"

#define  SMG_RCC_GPIO RCC_AHB1Periph_GPIOC
#define  SMG_GPIO     GPIOC

//74HC138
#define  HC138_A(val)   GPIO_WriteBit(SMG_GPIO, GPIO_Pin_3, (BitAction)val)
#define  HC138_B(val)   GPIO_WriteBit(SMG_GPIO, GPIO_Pin_4, (BitAction)val)
#define  HC138_C(val)   GPIO_WriteBit(SMG_GPIO, GPIO_Pin_5, (BitAction)val)

//74HC595
#define  HC595_SI(val)  GPIO_WriteBit(SMG_GPIO, GPIO_Pin_0, (BitAction)val)
#define  HC595_RCK(val) GPIO_WriteBit(SMG_GPIO, GPIO_Pin_1, (BitAction)val)
#define  HC595_SCK(val) GPIO_WriteBit(SMG_GPIO, GPIO_Pin_2, (BitAction)val)

void SMG_Init(void);
void HC595_Send(uint8_t dat);
void SMG_Sele(uint8_t index);


#endif

