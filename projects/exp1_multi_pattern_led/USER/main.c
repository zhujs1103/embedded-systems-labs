/*************************************************************
Experiment: multi-pattern LED running light

Hardware:
  ARM P12 -> LED P2
  PF0~PF7 -> LED1~LED8

Function:
  The program loops through several LED patterns automatically:
  single running light, reverse running light, ping-pong light,
  center spread, center gather, odd/even blink and fill/clear.
**************************************************************/
#include "stm32f4xx.h"
#include "delay.h"

#define LED_ACTIVE_LOW      0
#define LED_DELAY_MS        120
#define PATTERN_HOLD_MS     180
#define PATTERN_REPEAT      2

static const uint16_t g_single_left[] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080
};

static const uint16_t g_single_right[] = {
    0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001
};

static const uint16_t g_ping_pong[] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002
};

static const uint16_t g_spread[] = {
    0x0018, 0x0024, 0x0042, 0x0081
};

static const uint16_t g_gather[] = {
    0x0081, 0x0042, 0x0024, 0x0018
};

static const uint16_t g_odd_even[] = {
    0x0055, 0x00AA, 0x0055, 0x00AA
};

static const uint16_t g_fill_clear[] = {
    0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF,
    0x007F, 0x003F, 0x001F, 0x000F, 0x0007, 0x0003, 0x0001, 0x0000
};

static void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_Write(GPIOF, 0x0000);
}

static void LED_Show(uint16_t data)
{
#if LED_ACTIVE_LOW
    data = ~data;
#endif
    GPIO_Write(GPIOF, data & 0x00FF);
}

static void LED_Play(const uint16_t pattern[], uint8_t length, uint16_t delay_ms)
{
    uint8_t i;

    for (i = 0; i < length; i++) {
        LED_Show(pattern[i]);
        Delay_Ms(delay_ms);
    }

    LED_Show(0x0000);
    Delay_Ms(PATTERN_HOLD_MS);
}

static void LED_BlinkAll(uint8_t times)
{
    uint8_t i;

    for (i = 0; i < times; i++) {
        LED_Show(0x00FF);
        Delay_Ms(160);
        LED_Show(0x0000);
        Delay_Ms(160);
    }
}

int main(void)
{
    uint8_t repeat;

    Delay_Init();
    LED_Init();
    LED_BlinkAll(5);

    while (1) {
        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_single_left, sizeof(g_single_left) / sizeof(g_single_left[0]), LED_DELAY_MS);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_single_right, sizeof(g_single_right) / sizeof(g_single_right[0]), LED_DELAY_MS);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_ping_pong, sizeof(g_ping_pong) / sizeof(g_ping_pong[0]), LED_DELAY_MS);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_spread, sizeof(g_spread) / sizeof(g_spread[0]), 180);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_gather, sizeof(g_gather) / sizeof(g_gather[0]), 180);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_odd_even, sizeof(g_odd_even) / sizeof(g_odd_even[0]), 180);
        }

        for (repeat = 0; repeat < PATTERN_REPEAT; repeat++) {
            LED_Play(g_fill_clear, sizeof(g_fill_clear) / sizeof(g_fill_clear[0]), 90);
        }

        LED_BlinkAll(3);
    }
}
