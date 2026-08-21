#include "smg.h"
#include "delay.h"

#define SEG_BLANK_INDEX 10U

static const uint8_t g_digit_code[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F,
    0x00
};

static void SMG_SelectPosition(uint8_t pos)
{
    HC138_A(pos & 0x01U);
    HC138_B((pos >> 1) & 0x01U);
    HC138_C((pos >> 2) & 0x01U);
}

void SMG_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(SMG_RCC_GPIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SMG_GPIO, &GPIO_InitStructure);

    SMG_SelectPosition(0);
    SMG_ShowBlank();
}

void HC595_Send(uint8_t data)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        HC595_SI((data & 0x80U) ? 1 : 0);
        HC595_SCK(0);
        Delay_Us(1);
        HC595_SCK(1);
        Delay_Us(1);
        data <<= 1;
    }

    HC595_RCK(0);
    Delay_Us(3);
    HC595_RCK(1);
}

void SMG_ShowDigit(uint8_t digit)
{
    if (digit > 9U)
    {
        digit = 9U;
    }

    SMG_SelectPosition(0);
    HC595_Send(g_digit_code[digit]);
}

void SMG_ShowBlank(void)
{
    SMG_SelectPosition(0);
    HC595_Send(g_digit_code[SEG_BLANK_INDEX]);
}

void SMG_Sele(uint8_t index)
{
    SMG_ShowDigit(index);
}
