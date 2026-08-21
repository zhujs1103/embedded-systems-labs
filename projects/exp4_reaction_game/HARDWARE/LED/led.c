#include "led.h"
#include "delay.h"

volatile uint8_t led_flow_flag = 0;

static const uint16_t g_score_bar[] = {
    0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
    0x001F, 0x003F, 0x007F, 0x00FF, 0x00FF
};

void LED_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    LED_Set(0x0000);
}

void LED_Set(uint16_t data)
{
    GPIO_Write(GPIOF, data & 0x00FF);
}

void LED_ShowScore(uint8_t score)
{
    if (score >= (sizeof(g_score_bar) / sizeof(g_score_bar[0])))
    {
        score = (uint8_t)((sizeof(g_score_bar) / sizeof(g_score_bar[0])) - 1U);
    }

    LED_Set(g_score_bar[score]);
}

void LED_BlinkAll(uint8_t times, uint16_t delay_ms)
{
    uint8_t i;

    for (i = 0; i < times; i++)
    {
        LED_Set(0x00FF);
        Delay_Ms(delay_ms);
        LED_Set(0x0000);
        Delay_Ms(delay_ms);
    }
}
