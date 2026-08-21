#include "led.h"
#include "delay.h"

volatile uint8_t led_flow_flag = 0;

void LED_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    LED_SET(0x0000);
}

void LED_MainFlowStep(void)
{
    static uint8_t pos = 0;

    LED_SET((uint16_t)(1U << pos));
    Delay_Ms(120);

    pos++;
    pos &= 0x07;
}

void LED_TrafficFlash(void)
{
    uint8_t i;

    for (i = 0; i < 5; i++)
    {
        LED_SET(0x0021);
        Delay_Ms(300);
        LED_SET(0x0012);
        Delay_Ms(300);
        LED_SET(0x000C);
        Delay_Ms(300);
        LED_SET(0x0000);
        Delay_Ms(100);
    }
}

void LED_Flow(void)
{
    LED_MainFlowStep();
}
